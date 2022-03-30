import { io, Socket } from "socket.io-client";

//const IP_ADDRESS = '192.168.7.2'
const IP_ADDRESS = 'localhost'

export namespace RemoteCarControl {

  let _socket:Socket;

  let _errorDiv:HTMLDivElement;
  let _errorText:HTMLElement;
  let _statusText:HTMLElement;

  export function initialize(document:Document){
    _errorDiv = document.getElementById("error-box") as HTMLDivElement
    _errorText = document.getElementById("error-text") as HTMLElement
    _statusText = document.getElementById("status") as HTMLElement

  }

  export async function connect(){
    _socket = io('http://'+ IP_ADDRESS +':8088/',{})
    _socket.on("connect", async ()=>{
      
      // disable error
      _errorDiv.style.display = 'none'
      //notifyUpdateUI()
      console.log("client: connected to server")
      //alert("connected to server!")
    })
    
/*
    // ==== DEBUG =====
    // log every message
    _socket.on('message',(message:string)=>{
      console.log("Message recieved: " + message)
      _socket.send("Test message from client")
    })
    // log every events
    _socket.onAny((eventName:string)=>{
      console.log("event: " + eventName)
    })
*/
/*
    // generalization of error displaying
    _socket.on('emitError',(errorMessage:string) => {
      _errorDiv.style.display = 'block'
      _errorText.innerHTML = errorMessage
    })
*/
    _socket.on('connect_error',()=>{
      // happens when cannot connect to server
      _errorDiv.style.display = 'block'
      _errorDiv.style.backgroundColor = 'red'
      _errorText.innerHTML = "Cannot connect to server, make sure the node server is on"
    })

    _socket.on('disconnect',()=>{
      _errorDiv.style.display = 'block'
      _errorDiv.style.backgroundColor = 'red'
      _errorText.innerHTML = "Server disconnected, trying to reconnect"
      //console.log("disconnect: " )
    })

    _socket.on("networkModuleTimeout",()=>{
      _errorDiv.style.display = 'block'
      _errorDiv.style.backgroundColor = 'yellow'
      _errorText.innerHTML = "BeatBox C program not alive, but node server reachable"
    })
    _socket.on("networkModuleReconnect",()=>{
      _errorDiv.style.display = 'none'
      //notifyUpdateUI()
    })

  }

  export async function fetchAllValueUpdates(){
    // await updateVolume()
    // await updateTempo()
    // await updateBeatName()
  }

  export function remoteShutdown(){
    _socket.emit("remoteShutdown")
  }
}