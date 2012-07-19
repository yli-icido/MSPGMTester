#include "stdafx.h"

#include "PGMReceiver.h"
#pragma message (__FILE__ ": warning 4996 has been disableed" )
#pragma warning ( disable: 4996 )

using namespace std;


PGMReceiver::PGMReceiver() : 
mInitDone( false ),
mIsToQuit( false ),
mNetwork( PGM_MULTICAST_ADDRESS ),
mPort( DEFAULT_DATA_DESTINATION_PORT ),
mClientSocket( NULL )
{
}

PGMReceiver::~PGMReceiver()
{
    shutdown();
}

int PGMReceiver::initVar()
{
    mInitDone = false;
    mIsToQuit = false;
    mNetwork = PGM_MULTICAST_ADDRESS;
    mPort = DEFAULT_DATA_DESTINATION_PORT;
    mClientSocket = NULL;
    return PGM_SUCCESS;
}

void PGMReceiver::usage()
{
}

int PGMReceiver::init()
{
    mInitDone = true;
    return PGM_SUCCESS;
}

int PGMReceiver::connect()
{
    int retval = PGM_FAILURE;
    if ( !mInitDone ) 
    {
        return PGM_FATAL;
    }

    do
    {
        SOCKET        s;
        SOCKADDR_IN   salocal, sasession;
        int           sasessionsz, dwSessionPort;

        s = socket (AF_INET, SOCK_RDM, IPPROTO_RM);

        //
        // The bind port (dwSessionPort) specified should match that
        // which the sender specified in the connect call
        //
        dwSessionPort = 0;
        salocal.sin_family = AF_INET;
        salocal.sin_port   = htons (dwSessionPort);    
        salocal.sin_addr.s_addr = inet_addr ("234.5.6.7");

        bind (s, (SOCKADDR *)&salocal, sizeof(salocal));

        //
        // Set all relevant receiver socket options here
        //

        listen (s, 10);

        sasessionsz = sizeof(sasession);
        mClientSocket = accept (s, (SOCKADDR *)&sasession, &sasessionsz);

        //
        // accept will return the client socket and we are now ready
        // to receive data on the new socket!
        //

        retval = PGM_SUCCESS;

    } while ( false );

    return retval;
}

int PGMReceiver::receive()
{
    int retval = PGM_FATAL;
    bool isTerminated = false;
    do 
    {
        FILE* pFileToWrite = NULL;
        char fileToWrite[11];
        int rCounter = 0;
        char cCounter[3];

        LONG        BytesRead;
        char* buffer = new char[ PGM_BUFFER_SIZE ];
        BytesRead = recv (mClientSocket, buffer, PGM_BUFFER_SIZE, 0);
        if (BytesRead == 0)
        {
            fprintf(stdout, "Session was terminated\n");
        }
        else if (BytesRead == SOCKET_ERROR)
        {
            fprintf(stderr, "recv() failed: Error = %d\n", WSAGetLastError());
        }
        else if ( strcmp( buffer, "start" ) == 0 )
        {
            printf ("start\n");
            if ( pFileToWrite != NULL )
            {
                fclose( pFileToWrite );
                pFileToWrite = NULL;
            }
            strcpy( fileToWrite, "received" );
            _itoa( rCounter, cCounter, 10 );
            pFileToWrite = fopen( strcat( fileToWrite, cCounter ), "w" );
            rCounter++;
        }
        else if ( strcmp( buffer, "end" ) == 0 )
        {
            printf("end\n");
            fclose( pFileToWrite );
            pFileToWrite = NULL;
        }
        else if ( strcmp( buffer, "-q" ) == 0 )
        {
            if ( pFileToWrite != NULL )
            {
                fclose( pFileToWrite );
                pFileToWrite = NULL;
            }
            isTerminated = true;
        }
        else if ( pFileToWrite != NULL )
        {
            fwrite( buffer, 1, BytesRead, pFileToWrite );
        }
        else
        {
            fprintf(stderr, "output file not open\n");
        }

        retval = PGM_SUCCESS;

    } while ( !isTerminated );

    return retval;
}

int PGMReceiver::shutdown()
{
    return PGM_SUCCESS;
}

// take the complete option string, analyse if the user input option is valid
int PGMReceiver::analyseOptions( string& options )
{
    int retval = PGM_FAILURE;
    do
    {

        retval = PGM_SUCCESS;

    } while ( false );

    return retval;
}

int PGMReceiver::verifyOptions( std::map< char, std::string >& options )
{
    int retval = PGM_FAILURE;
    retval = PGM_SUCCESS;

    return retval;
}