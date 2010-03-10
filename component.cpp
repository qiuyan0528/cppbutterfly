/*
    Copyright (c) 2010 Gonzalo Diethelm

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
*/
 
#include <zmq.hpp>
 
#include <assert.h>
#include <stdio.h>
 
int main (int argc, char *argv [])
{
  // Parse command line arguments.
  if (argc != 5) {
    printf ("usage: component <out-interface> "
            "<inp-interface> <processing-time [ms]> <name>\n");
    return 1;
  }
  const char *out_interface = argv [1];
  const char *inp_interface = argv [2];
  int processing_time = atoi (argv [3]);
  const char *cname = argv [4];
        
  // Create the 0MQ infrastructure.
  zmq::context_t ctx (1, 1);
  zmq::socket_t inp_socket (ctx, ZMQ_UPSTREAM);
  zmq::socket_t out_socket (ctx, ZMQ_DOWNSTREAM);

  inp_socket.connect (inp_interface);
  out_socket.connect (out_interface);
    
  // Main event loop.
  int count = 0;
  while (true) {
    zmq::message_t msg;
    inp_socket.recv (&msg);
 
    // Simulate processing, i.e. sleep for the specified time.
#if 1
    Sleep (processing_time);
#else
    usleep (processing_time * 1000);
#endif
 
    out_socket.send (msg);
  }
}
