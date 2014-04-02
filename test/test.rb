#!/usr/bin/ruby

# Simple test for IoServer

require 'socket'

hostname = 'localhost'
port = 2005

loop do
  s = TCPSocket.open(hostname, port)

  # <count of command bytes> <count of response bytes> <command bytes>
  str = "batt_u;0;100;"
  s.puts str
  data = s.gets
  #int_data = (data[0] * 256 + data[1].to_i)
  puts data.inspect
  s.close               # Close the socket when done

  sleep 2

end
