# win32_protocol
Header-only URI Protocol Handler for windows

#How to use it?
Here's all the functions you need to know about!
```C++
void handle_cmd( const char* cmd_raw ); // you should pass lpCmdLine to this function for everything to function properly
protocol_t* create_protocol( std::string protocol_name ); // creates a new protocol and registers it if it wasnt registered before so it can be used ( protocol_name://x/y/z/ )
void protocol_t::add_callback( function<void( proto_cmd_t& )> handler ); // lets you add a handler for the protocol
```

Just don't forget that the handlers you register should be thread-safe.

#Example
It's very simple to use!
```C++

INT WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
			 PCHAR lpCmdLine, INT nCmdShow )
{
	protocol::protocol_t * p = protocol::create_protocol( "test" );

	p->add_callback( []( protocol::proto_cmd_t& cmd )
	{
		string data = "Hello!\nJust got a message via protocol: " + cmd.protocol_name + "\nArgs:\n";
		
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
```
After you launched it atleast once, clicking on [test://Success](test://Success) will print 
```
Hello!
Just got a message via protocol: test
Args:
[0] Success!
```
on the parent process (or if theres no already launched copy, the newly launched one).