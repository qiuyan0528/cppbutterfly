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
 
#include "zmq.hpp"
 
#include <assert.h>
#include <stdio.h>
 
int main (int argc, char *argv [])
{
  // Parse command line arguments.
  if (argc != 4) {
    printf ("usage: send_requests <out-interface> "
            "<sync-interface> <transaction-count>\n");
    return 1;
  }
  const char *out_interface = argv [1];
  const char *sync_interface = argv [2];
  int transaction_count = atoi (argv [3]);
  assert (transaction_count >= 1);

  // Create the 0MQ infrastructure.
  zmq::context_t ctx (1, 1);
  zmq::socket_t inp_socket (ctx, ZMQ_UPSTREAM);
  zmq::socket_t out_socket (ctx, ZMQ_DOWNSTREAM);
  inp_socket.bind (sync_interface);
  out_socket.bind (out_interface);

  while (true) {
    // Start after any key is hit.
    printf ("Hit ENTER to start!\n");
    getchar ();
    printf ("Running...\n");
 
    // Get timer running here.
    void* timer = zmq_stopwatch_start ();
 
    // Send messages. We don't bother to fill any data in.
    zmq::message_t msg (100);
    for (int counter = 0; counter != transaction_count; ++counter) {
      if ((counter % 10) == 0) {
	fprintf(stderr, "Sent %d messages so far\n", counter);
      }
      out_socket.send (msg);
    }
 
    // Wait for synchronisation message from 'receive_replies' application.
    zmq::message_t sync;
    inp_socket.recv (&sync);
 
    // Get elapsed time in useconds.
    unsigned long elapsed = zmq_stopwatch_stop (timer);

    // Print the results.
    printf ("Throughput: %.3f transactions/second.\n\n",
            ((double) transaction_count * 1000000) / elapsed);
  }
}
