#include "stdafx.h"

#include "PGMSender.h"
#pragma message (__FILE__ ": warning 4996 has been disableed" )
#pragma warning ( disable: 4996 )

using namespace std;


PGMSender::PGMSender():
mInitDone( false ),
mIsConnected( false ),
mIsToQuit( false ),
mNetwork( PGM_MULTICAST_ADDRESS ),
mPort( DEFAULT_DATA_DESTINATION_PORT ),
mSocket( NULL )
{
}

PGMSender::~PGMSender()
{
    shutdown();
}

int PGMSender::initVar()
{
    mInitDone = false;
    mIsConnected = false;
    mIsToQuit = false;
    mNetwork = PGM_MULTICAST_ADDRESS;
    mPort = DEFAULT_DATA_DESTINATION_PORT;
    mSocket = NULL;
    return PGM_SUCCESS;
}

int PGMSender::init()
{
    mInitDone = true;
    return PGM_SUCCESS;
}

int PGMSender::shutdown()
{
    return PGM_SUCCESS;
}

int PGMSender::connect()
{
    int retval = PGM_FAILURE;
    if ( !mInitDone ) 
    {
        cerr << "pgm not initialized" << endl;
        return PGM_FATAL;
    }
    string userInput;
    int retryCnt = 0;

    do
    {
        retval = createSocket();
        if ( retval == PGM_FATAL )
            break;
        else if ( retval != PGM_SUCCESS )
        {
            if ( retryCnt < 5 )
            {
                retryCnt++;
                initVar();
                continue;
            }
            else
                break;
        }
    } while ( (retval != PGM_SUCCESS) && (mIsToQuit == false) );
    
    return retval;
}

int PGMSender::send()
{
    int retval = PGM_FAILURE;
    string userInput;
    int sCounter = 0;
    for (;;)
    {
        // expect a file name
        cout << "please input the full file name to send" << endl;
        cout << "for more than one files, separate the file names with \";\"" << endl;
        cout << "   -q to quit" << endl;
        getline( cin, userInput );
        if ( userInput == "-q" )
        {
            retval = PGM_SUCCESS;
            break;
        }
        vector< string > filenames;
        filenames.push_back( "C:\\Users\\yli\\Downloads\\mouse.sso" );
//         string separator(";");
//         retval = PGMUtils::intoTokens( userInput, separator, false, filenames );
//         if ( retval != PGM_SUCCESS )
//             break;

        FILE* pFileToSend = NULL;
        char* buffer = new char[ PGM_BUFFER_SIZE ];
        char cCounter[3];
        _itoa( sCounter, cCounter, 10 );
        char fileToWrite[10];
        strcpy( fileToWrite, "sent" );
        FILE* pFileToWrite = fopen( strcat( fileToWrite, cCounter ), "w" );
        sCounter++;
        LONG        error;
        //:
        error = ::send (mSocket, "start", strlen( "start" ), NULL );
        if (error == SOCKET_ERROR)
        {
            fprintf (stderr, "send() failed: Error = %d\n", WSAGetLastError());
            retval = PGM_FATAL;
            break;
        }

        // send a "start" indicate a transfer session started

        for ( size_t i = 0; i < filenames.size(); i++ )
        {
            pFileToSend = fopen( filenames[i].c_str(), "rb" );
            if (pFileToSend == NULL) 
            {
				cerr << "Error opening file: " << filenames[i] << endl;
                continue;
            }
            fseek( pFileToSend, 0, SEEK_END );
            size_t fileSize = ftell( pFileToSend );
            size_t curPos = 0;
            size_t sizeToRead = PGM_BUFFER_SIZE;
            rewind( pFileToSend );
            size_t readResult = 0;
            while ( !feof( pFileToSend ) && ( curPos < fileSize ) )
            {
                if ( fileSize - curPos < PGM_BUFFER_SIZE )
                {
                    sizeToRead = fileSize - curPos;
                }

                readResult = fread( buffer, 1, sizeToRead, pFileToSend );
                if ( readResult != sizeToRead )
                {
                    cerr << "error reading file at: " << ftell(pFileToSend) << endl;
                    cerr << "sizeToRead:" << sizeToRead << ", actual read:" << readResult << endl;
                    cerr << "error code: " << ferror( pFileToSend ) << ", eof:" << feof( pFileToSend) << endl;
                }
                curPos += readResult;
                error = ::send ( mSocket, buffer, readResult, NULL );
                if (error == SOCKET_ERROR)
                {
                    fprintf (stderr, "send() failed: Error = %d\n", WSAGetLastError());
                    retval = PGM_FATAL;
                    break;
                }

                fwrite( buffer, 1, readResult, pFileToWrite );
            }
            fclose( pFileToSend );
            pFileToSend = NULL;
        }
        // send a "end" to indicate transfer session end
        error = ::send( mSocket, "end", strlen( "end" ), NULL );
        fclose( pFileToWrite );
    }

    return retval;
}

int PGMSender::createSocket()
{
    int retval = PGM_FAILURE;
    do 
    {
        SOCKADDR_IN   salocal, sasession;
        int           dwSessionPort;

        mSocket = socket (AF_INET, SOCK_RDM, IPPROTO_RM);
        if ( mSocket == INVALID_SOCKET )
        {
            fprintf (stderr, "socket() failed: Error = %d\n", WSAGetLastError());
            break;
        }

        salocal.sin_family = AF_INET;
        salocal.sin_port   = htons (0);    // Port is ignored here
        salocal.sin_addr.s_addr = htonl (INADDR_ANY);

        if ( !bind (mSocket, (SOCKADDR *)&salocal, sizeof(salocal)) )
        {
            fprintf (stderr, "bind() failed: Error = %d\n", WSAGetLastError());
            break;
        }

        //
        // Set all relevant sender socket options here
        //

        //
        // Now, connect <entity type="hellip"/>
        // Setting the connection port (dwSessionPort) has relevance, and
        // can be used to multiplex multiple sessions to the same
        // multicast group address over different ports
        //
        dwSessionPort = 0;
        sasession.sin_family = AF_INET;
        sasession.sin_port   = htons (dwSessionPort);
        sasession.sin_addr.s_addr = inet_addr ("234.5.6.7");

        if ( !::connect (mSocket, (SOCKADDR *)&sasession, sizeof(sasession)) )
        {
            fprintf (stderr, "connect() failed: Error = %d\n", WSAGetLastError());
            break;
        }

        //
        // We're now ready to send data!
        //
        mIsConnected = true;
        retval = PGM_SUCCESS;

    } while ( false );

    return PGM_FATAL;
}

void PGMSender::usage()
{
}

// take the complete option string, analyse if the user input option is valid
int PGMSender::analyseOptions( string& options )
{
    const string supportedOptions("nsprfKNlih?q");
    int retval = PGM_FAILURE;
    do
    {
        map< char, string > optionPairs;

//         retval = PGMUtils::intoOptions( options, optionPairs );
        if ( retval != PGM_SUCCESS )
        {
            break;
        }

        map< char, string >::iterator itr = optionPairs.begin();
        while ( itr != optionPairs.end() )
        {
            string optionValue = itr->second;
            char optionSwitch = itr->first;
            switch ( optionSwitch )
            {

            case 'q':
                mIsToQuit = true;
                break;
            default:
                cout << "wrong parameter: -" << optionSwitch << " " << optionValue << endl;
                usage();
                break;
            }

            itr++;
        }

        retval = verifyOptions( optionPairs );
        if ( retval != PGM_SUCCESS )
        {
            break;
        }

        retval = PGM_SUCCESS;

    } while ( false );
    
    return retval;
}

int PGMSender::verifyOptions( std::map< char, std::string >& options )
{
    int retval = PGM_FAILURE;
    retval = PGM_SUCCESS;

    return retval;
}
