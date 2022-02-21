/**
 * A simple udp socket server referenced
 * https://nodejs.org/api/dgram.html
 */

import dgram from "dgram"

// export makes function usable when imported from to elsewhere
export function simpleUDPserver(){
  const server = dgram.createSocket("udp4");

  server.on('listening', () => {
    const address = server.address();
    console.log(`server listening ${address.address}:${address.port}`);
  });

  server.on("message",(msg:string, rinfo:dgram.RemoteInfo)=>{
    console.log("Server just recieved message:\n" +msg)
    // showcasing two ways to print variable
    console.log("Message was from: " + rinfo.address + ":" + rinfo.port)

    // close server after recieving one message
    server.close();
  })

  server.bind(12345);
}
