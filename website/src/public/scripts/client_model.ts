import { io, Socket } from "socket.io-client";
import { MapUpdate } from "../../utils";
import { addMapUpdate } from "./client_ui";

const REMOTE_ADDRESS = '192.168.7.2'
const LOCAL_ADDRESS = 'localhost'
let ipAddress = REMOTE_ADDRESS

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
    _socket = io('http://'+ ipAddress +':8088/',{})
    _socket.on("connect", async ()=>{
      
      // disable error
      _errorDiv.style.display = 'none'
      //notifyUpdateUI()
      console.log("client: connected to server!")
      //alert("connected to server!")
    })
    

    // ==== DEBUG =====
    // log every message
    _socket.on('message',(message:string)=>{
      console.log("Message recieved: " + message)
      //_socket.send("Test message from client")
    })
    // log every events
    _socket.onAny((eventName:string)=>{
      console.log("event: " + eventName)
    })

/*
    // generalization of error displaying
    _socket.on('emitError',(errorMessage:string) => {
      _errorDiv.style.display = 'block'
      _errorText.innerHTML = errorMessage
    })
*/
    _socket.on('updateUI',(message:string)=>{
      //JSON.parse()
      console.log("updating UI")
      const jsonStr = message.split(">>")[1]
      const updatePayload = JSON.parse(jsonStr) as MapUpdate
      console.log(updatePayload)
      addMapUpdate(updatePayload)
    })


    _socket.on('connect_error',()=>{
      // happens when cannot connect to server
      _errorDiv.style.display = 'block'
      _errorDiv.style.backgroundColor = 'red'
      _errorText.innerHTML = "Cannot connect to server, make sure the node server is on"
      
      if(ipAddress == LOCAL_ADDRESS){
        ipAddress = REMOTE_ADDRESS
      } else {
        ipAddress = LOCAL_ADDRESS
      }
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

  export function moveLeft(){
    _socket.emit("relayMessage","moveLeft")
  }

  export function moveRight(){
    _socket.emit("relayMessage","moveRight")
  }

  export function moveFront(){
    _socket.emit("relayMessage","moveFront")
  }

  export function moveBack(){
    _socket.emit("relayMessage","moveBack")
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