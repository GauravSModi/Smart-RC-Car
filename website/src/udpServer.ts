import { Socket } from "socket.io"
import * as dgram from "dgram"
import { EventEmitter } from "stream";

const PORT = 12345;
const KEEPALIVE_MAX_MS = 100;
const KEEPALIVE_CHECK_INTERVAL_MS = 500;
const HOST = '127.0.0.1' // the loopback address or 'localhost'
//const POLL_TIME_INTERVAL_MS = 1000;

// mainly used to communicate with c program
export namespace UDPRelay {
  // event driven
  // includes events:
  // "alive" : emit boolean either true or false
  export let events:EventEmitter;
  let _wasAlive:boolean = false;
  let _tcp_sockets:any[] = []

  export function initialize(){
    events = new EventEmitter()
    const keepAliveSocket = dgram.createSocket('udp4')
    keepAliveSocket.bind(11000)
    
    // check C program alive status
    const checkAlive = async function(){
      const status = await new Promise<number>((resolve,reject)=>{
        keepAliveSocket.once("message",(message:Buffer,rinfo:dgram.RemoteInfo)=>{
          //console.log("Once keepAlive: ");
          //console.log(message.toString('utf-8'));
          //console.log(rinfo)
          resolve(1)
        })
  
        keepAliveSocket.send("alive", PORT, HOST, (error:Error | null, bytes:number)=>{})
        setTimeout(()=>{
          keepAliveSocket.removeAllListeners("message")
          resolve(0)
        },KEEPALIVE_MAX_MS)
      }) 
      let isAlive = (status == 1)
      // reconnection
      if(_wasAlive != isAlive){
        if(_wasAlive){
          events.emit("disconnect")
        } else {
          events.emit("reconnect")
        }
      }
      // update status
      _wasAlive = isAlive
      events.emit("alive",_wasAlive)
      if(!_wasAlive){
        events.emit("timeout")
      }

      //console.log("status: " + status)
      setTimeout(checkAlive,KEEPALIVE_CHECK_INTERVAL_MS)
    }
    // start loop
    checkAlive()
  }

  export function relayToNetworkModule(socket:Socket){
    
    // =========> FROM CLIENT(C PROGRAM) 
    const clientIO = dgram.createSocket('udp4')
    
    // DEBUG
    // log message from network module 
    /*
    clientIO.on('message', (message:string, remote:dgram.RemoteInfo)=>{
      //console.log(remote)
      console.log("Client Recieved: [" + message + "]")
      //client.close()
    })*/

    const clientBroadCast = dgram.createSocket('udp4')
    clientBroadCast.on('message',(message:Buffer, remote:dgram.RemoteInfo)=>{
      const messageStr = message.toString('utf8')
      console.log("Broadcast Recieved: ["+messageStr+"]")
      socket.emit("updateUI",messageStr) 
    })

    clientBroadCast.send('subscribe', PORT, HOST, (error:Error | null, bytes:number)=>{})

    // =========> FROM SOCKET(WEB UI) TO CLIENT(C PROGRAM)
    console.log("User Connected")
    //        ==> SHUTDOWN PROCEDURE
    socket.on("remoteShutdown", ()=>{
      clientIO.send("stop", PORT,HOST,(error:Error | null, bytes:number)=>{
      })
      events.on("timeout",()=>{
        console.log("Closing sockets")
        disconnect()
        console.log("Shutting down")
        process.exit(0)
      })
    })

    socket.on("relayMessage",(message:string)=>{
      if(message && message != ""){
        clientIO.send(message, PORT,HOST,(error:Error | null, bytes:number)=>{
          console.log(message);
        })
      }
    })

    // =========> NODE SERVER TO NODE CLIENT
    const onTimeout = ()=>{socket.emit("networkModuleTimeout")}
    events.on("timeout",onTimeout)
    const onReconnect = ()=>{
      socket.emit("networkModuleReconnect")
      console.log("C Program reconnected")
    }
    events.on("reconnect",onReconnect)
      
    /*
    const intervalID = setInterval(async ()=>{
      // request info from net module
      let result = await requestInput(clientIO,"uptime").catch(console.log)
      if(result){
        socket.emit('updateUpTime', parseInt(result[0]))
      }
    },POLL_TIME_INTERVAL_MS)
    */

    // remove listeners on disconnect
    socket.on("disconnect",()=>{
      clientBroadCast.send('unsubscribe', PORT, HOST, (error:Error | null, bytes:number)=>{})
      events.removeListener("timeout",onTimeout)
      events.removeListener("reconnect",onReconnect)
      //clearInterval(intervalID)
    })
    _tcp_sockets.push(socket)
  }

  // referenced udp_server.js in 10-UdpRelay by Brian Fraser

  function requestInput(client:dgram.Socket,commandName:string): Promise<string[]>{
    return new Promise<string[]>((resolve,reject)=>{
      const timeoutListener = ()=>{
        //console.log("["+commandName+"]TIMED OUT!")
        client.removeListener('message',listener)
        reject("["+commandName+"] timed out")
      }
      events.once("timeout",timeoutListener)
      
      const listener = (message:Buffer,rinfo:dgram.RemoteInfo)=>{
        let strBuffer = message.toString('utf8')
        let splits = strBuffer.split(' ')
        
        if(splits[0].match(commandName)){
          //console.log(strBuffer)
          events.removeListener("timeout",timeoutListener)
          client.removeListener('message',listener)
          splits.shift();
          
          resolve(splits)
        }/* else {
          console.log("different message recieved:["+message+"]["+strBuffer+"]")
        }*/
      }

      client.addListener('message',listener)
    
      client.send(commandName,PORT,HOST,(error:Error | null, bytes:number)=>{
      })
      
    })
  }

  export function disconnect(){
    _tcp_sockets.forEach(socket => {
      socket.disconnect(true)
    }); 
  }

}

