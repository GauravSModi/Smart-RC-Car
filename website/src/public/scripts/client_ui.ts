
import { RemoteCarControl } from './client_model'


// Get reference to DOM elements
// status 
//const statusText = document.getElementById("status") as HTMLElement

// Mode switches
const modeButtonsDiv = document.getElementById("mode-button") as HTMLDivElement

// Volumes
const volumeText = document.getElementById("volumeid") as HTMLInputElement
const volumeDownButton = document.getElementById("volumeDown") as HTMLButtonElement
const volumeUpButton = document.getElementById("volumeUp") as HTMLButtonElement

// Tempo
const tempoText = document.getElementById("tempoid") as HTMLInputElement
const tempoDownButton = document.getElementById("tempoDown") as HTMLButtonElement
const tempoUpButton = document.getElementById("tempoUp") as HTMLButtonElement

// Play sound
const soundButtonsDiv = document.getElementById("sound-buttons") as HTMLDivElement
// errors
const errorDiv = document.getElementById("error-box") as HTMLDivElement

// shutdown Button
const shutdownButton = document.getElementById("shutdown-button") as HTMLInputElement

async function main(){
  RemoteCarControl.initialize(document)
  
  RemoteCarControl.connect()
  //notifyUpdateUI()
  //alert("JS loaded successfully YAY")
  //statusText.innerHTML = "Device up for:\n" + await BeatBox.getUpTime()

  shutdownButton.onclick = function (event: MouseEvent) {
    RemoteCarControl.remoteShutdown()
  }

}

main()