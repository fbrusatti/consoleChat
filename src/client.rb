require 'socket'

PORT = 4848
SERVER_ADDRESS = '127.0.0.1'

class Client
  def initialize(server)
    @server = server
    @request = nil
    @response = nil
    listen
    send
    @request.join
    @response.join
  end

  # read from the console
  # with the enter key, send the message to the server
  def send
    puts 'Enter the username: '
    @request = Thread.new do
      loop {
        msg = $stdin.gets.chomp
        @server.puts( msg )
      }
    end
  end

  # listen the server responses
  # show them in the console
  def listen
    @response = Thread.new do
      loop {
        msg = @server.gets.chomp
        puts "#{msg}"
      }
    end
  end
end

server = TCPSocket.open(SERVER_ADDRESS, PORT)
Client.new( server )

