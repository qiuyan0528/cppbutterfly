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
  if (argc != 4) {
    printf ("usage: receive_replies <in-interface> "
            "<sync-interface> <transaction-count>\n");
    return 1;
  }
  const char *inp_interface = argv [1];
  const char *sync_interface = argv [2];
  int transaction_count = atoi (argv [3]);
  assert (transaction_count >= 1);
 
  // Create the 0MQ infrastructure.
  zmq::context_t ctx (1, 1);
  zmq::socket_t inp_socket (ctx, ZMQ_UPSTREAM);
  zmq::socket_t out_socket (ctx, ZMQ_DOWNSTREAM);

  inp_socket.bind (inp_interface);
  out_socket.connect (sync_interface);

  while (true) {
    // Wait for all the replies.
    for (int counter = 0; counter != transaction_count; ++counter) {
      zmq::message_t msg;
      inp_socket.recv (&msg);
      if ((counter % 10) == 0) {
	fprintf(stderr, "Received %d messages so far\n", counter);
      }
    }
    
    // Send the synchronisation messages to 'send_requests' application.
    zmq::message_t sync(1);
    out_socket.send (sync);
  }
}
