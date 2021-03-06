// OpenPGMConsoleTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PGMReceiver.h"
#include "PGMSender.h"

#pragma message (__FILE__ ": warning 4996 has been disableed" )
#pragma warning ( disable: 4996 )

using namespace std;

int _tmain( int argc, _TCHAR* argv[] )
{
    int retval = EXIT_FAILURE;
    char userInputc[256];

    do 
    {
        fprintf (stderr, "type \"send\" to send packages\n");
        fprintf (stderr, "type \"receive\" to receive packages\n");
        fprintf (stderr, "type \"exit\" to exit\n");
        gets( userInputc );

        if ( strcmp( userInputc, "send" ) == 0 ) // send
        {
            do 
            {
                PGMSender* sender = new PGMSender();
                if ( sender == NULL ) break;

                if ( sender->init() != PGM_SUCCESS ) 
                    break;

                if ( sender->connect() != PGM_SUCCESS )
                    break;

                if ( sender->send() != PGM_SUCCESS )
                    break;

                retval = EXIT_SUCCESS;

            } while ( false );
        }
        else if ( strcmp( userInputc, "receive" ) == 0 ) // receive
        {
            do 
            {
                PGMReceiver* receiver = new PGMReceiver();
                if ( receiver == NULL ) break;

                if ( receiver->init() != PGM_SUCCESS )
                    break;

                if ( receiver->connect() != PGM_SUCCESS )
                    break;

                receiver->receive();

                if ( receiver->shutdown() != PGM_SUCCESS )
                    break;

                retval = EXIT_SUCCESS;

            } while ( false );
        }
        else if ( strcmp( userInputc, "exit" ) == 0 )
        {
            retval = EXIT_SUCCESS;
            break;
        }

    } while ( strcmp( userInputc, "exit" ) != 0 );

    return retval;
}
