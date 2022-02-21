/**
 * A simple udp socket client
 */

 import dgram from "dgram"

 export function simpleUDPclient(){
  const client = dgram.createSocket("udp4")
  // send message with parameters
  // send(message, offset, length, port, address)
  client.send("Hi Server!",0,10,12345,"localhost",()=>{

    // log after message is sent
    console.log("message is sent!")

    // close client after sending one message
    client.close()
  })
}