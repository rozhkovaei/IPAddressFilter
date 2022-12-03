#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

constexpr int IPADDRESS_PARTS = 4;

using namespace std;

namespace
{
    // ("",  '.') -> [""]
    // ("11", '.') -> ["11"]
    // ("..", '.') -> ["", "", ""]
    // ("11.", '.') -> ["11", ""]
    // (".11", '.') -> ["", "11"]
    // ("11.22", '.') -> ["11", "22"]

    std::vector<std::string> split( const string& str, char d )
    {
        std::vector<std::string> r;

        string::size_type start = 0;
        auto stop = str.find_first_of( d );

        while ( stop != string::npos )
        {
            r.push_back( str.substr( start, stop - start ) );

            start = stop + 1;
            stop = str.find_first_of( d, start );
        }

        r.push_back( str.substr( start ) );

        return r;
    }
}

class IPAddress
{
public:
    IPAddress( const std::vector<std::string>& address )
    {
        parts.reserve( IPADDRESS_PARTS );

        for ( auto& part : address )
        {
            parts.emplace_back( atoi( part.c_str() ) );
        }
    }

    int GetPartValue( int num )
    {
        if ( num >= 0 && num < IPADDRESS_PARTS )
        {
            return parts[ num ];
        }
        return -1;
    }

    bool operator > ( const IPAddress& addr ) const
    {
        for ( int i = 0; i < IPADDRESS_PARTS; ++i )
        {
            if ( parts[i] > addr.parts[ i ] )
                return true;

            if ( parts[ i ] < addr.parts[ i ] )
                return false;
        }
        
        return false;
    }

    friend ostream& operator<<( ostream& os, const IPAddress& addr );

private:

    vector<int> parts;
};

ostream& operator<<( ostream& os, const IPAddress& addr )
{
    bool begin = true;

    for ( auto& part : addr.parts )
    {
        if ( begin )
        {
            begin = false;
        }
        else
        {
            cout << ".";
        }
        cout << part;
    }

    cout << endl;

    return os;
}


class IPPool
{
public:
    IPPool() = default;

    void Add( const std::vector<std::string>& addr )
    {
        ip_addresses.emplace_back( make_shared<IPAddress>( addr ) );
    }

    void Filter()
    {
        for ( auto& ip : ip_addresses )
        {
            if ( ip->GetPartValue( 0 ) == 1 )
            {
                filter_1.push_back( ip );
            }
            if ( ip->GetPartValue( 0 ) == 46 && ip->GetPartValue( 1 ) == 70 )
            {
                filter_2.push_back( ip );
            }
            if ( ip->GetPartValue( 0 ) == 46 || ip->GetPartValue( 1 ) == 46 ||
                ip->GetPartValue( 2 ) == 46 || ip->GetPartValue( 3 ) == 46 )
            {
                filter_3.push_back( ip );
            }
        }
    }

    friend ostream& operator<<( ostream& os, const IPPool& addr );

    friend ostream& operator<<( ostream& os, const vector<shared_ptr<IPAddress>>& pool );

    vector<shared_ptr<IPAddress>>::iterator begin()
    {
        return ip_addresses.begin();
    }

    vector<shared_ptr<IPAddress>>::iterator end()
    {
        return ip_addresses.end();
    }

    vector<shared_ptr<IPAddress>> filter_1;
    vector<shared_ptr<IPAddress>> filter_2;
    vector<shared_ptr<IPAddress>> filter_3;

private:

    vector<shared_ptr<IPAddress>> ip_addresses;
};

ostream& operator<<( ostream& os, const IPPool& pool )
{
    for ( auto& addr : pool.ip_addresses )
    {
        cout << *addr;
    }
    return os;
}

ostream& operator<<( ostream& os, const vector<shared_ptr<IPAddress>>& pool )
{
    for ( auto& addr : pool )
    {
        cout << *addr;
    }
    return os;
}

int main(int argc, char const *argv[])
{
    try
    {
        IPPool ip_pool;

   //     ifstream file( "E://CProfessional/02_homework-12995-fb7660/ip_filter.tsv" );

        for ( string line; getline( cin, line );)
        {
            auto v = split(line, '\t');
            ip_pool.Add(split(v.at(0), '.'));
        }

        sort( ip_pool.begin(), ip_pool.end(), [] ( shared_ptr<IPAddress>& a, shared_ptr<IPAddress>& b ) {
            return *a > * b;

        } );

        ip_pool.Filter();

        cout << ip_pool;
        cout << ip_pool.filter_1;
        cout << ip_pool.filter_2;
        cout << ip_pool.filter_3;

        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231   
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const exception &e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}
