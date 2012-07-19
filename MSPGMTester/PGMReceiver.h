#ifndef PMGRECEIVER_H_
#define PMGRECEIVER_H_


class PGMReceiver
{
public:
    PGMReceiver();
    ~PGMReceiver();

    int     init();
    int     connect();
    int     receive();
    int     shutdown();

private:
    int     initVar();
    void    usage();
    int     analyseOptions( std::string& options );
    int     verifyOptions( std::map< char, std::string >& options );
    int     createSocket();

private:
    bool            mInitDone;
    bool            mIsToQuit;
    std::string     mNetwork;
    unsigned int    mPort;
    SOCKET          mClientSocket;
};
#endif // PMGRECEIVER_H_