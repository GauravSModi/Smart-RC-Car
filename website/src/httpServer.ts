import Express from "express"
import { AddressInfo } from "net"
import path from "path"
import * as SocketIO from "socket.io"
import { UDPRelay } from "./udpServer"

// referenced tutorial by WebDevSimplified, https://www.youtube.com/watch?v=SccSCuHhOw0
async function main(){
  const httpServer = Express()

  httpServer.get("/", (request, response)=>{
    console.log("Tests")
    response.status(200)
    // response.send("simple text response")
    // response.json({message: "sample json"})
    // response.download("beatboxServer.ts")
    //response.sendFile(path.join(__dirname,"public","index.html"))
    response.redirect("/static/index.html")
  })
//asd
  httpServer.use("/static",Express.static(path.join(__dirname,"public")))

  httpServer.get("*", (request, response)=>{
    response.status(404).redirect("/static/404.html")
  })

  const instance = httpServer.listen(8088,()=>{
    let serverInfo = instance.address() as AddressInfo
    if(!serverInfo){
      console.log("Fail to get server listening address")
    } else {
      let address = serverInfo.address
      if (address.match("::")){
        // :: refers to 127.0.0.1 / localhost
        // address = "localhost"
        // however, in our set, we can assume the server is run on BeagleBone
        // with ip 192.168.7.2 via ethernet-over-usb
        address = "192.168.7.2"
      }
      let port = serverInfo.port
      console.log("HTTP Server listening at %s:%d",address,port)
    }
  })

  // intialize UDP relay
  UDPRelay.initialize()

  // instance Server
  const ioServer = new SocketIO.Server(instance)
  ioServer.on("connection",(socket:SocketIO.Socket)=>{
    UDPRelay.relayToNetworkModule(socket)
  })
  
}

main()