#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include <iostream>
#include <string>
#include "protocol.h"
using namespace std;

void print_thread_safe(string& str)
{
	static bool console_open = false;
	static CRITICAL_SECTION cs;
	static bool cs_init = false;
	if( !cs_init )
		InitializeCriticalSection( &cs );

	if ( !console_open )
	{
		AllocConsole();
		freopen( "CONIN$", "rb", stdin );
		freopen( "CONOUT$", "wb", stdout );
		freopen( "CONOUT$", "wb", stderr );
	}

	EnterCriticalSection( &cs );
	cout << str.c_str();
	LeaveCriticalSection( &cs );
}

INT WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
			 PCHAR lpCmdLine, INT nCmdShow )
{
	protocol::protocol_t * p = protocol::create_protocol( "test" );

	p->add_callback( []( protocol::proto_cmd_t& cmd )
	{
		string data = "Hello!\nJust got a message via protocol: " + cmd.procol_name + "\nArgs:\n";
		
		int n = 0;
		for ( string& s : cmd.args )
			data += "[" + to_string(n++) + "] " + s + "\n";

		data += "\n";

		print_thread_safe( data );
	} );

	protocol::handle_cmd( lpCmdLine );
	
	print_thread_safe( string("") );

	while ( 1 );
	return 0;
}

