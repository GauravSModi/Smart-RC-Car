
import { RemoteCarControl } from './client_model'
import P5 from "p5"

// Get reference to DOM elements
// status 

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

  const gridCellWidth = 40

  new P5((p5:P5)=>{

    let xMiddle:number;
    let yMiddle:number;
    let timeStep:number = 0;
    const radius = 50;
    const size = 10;
    const speed = 0.1; // in radians
    const color = "#F5D1FF"

    p5.setup = () => {
      const canvas = p5.createCanvas(400,400)
      canvas.style("display","block")
      canvas.style("margin","auto")
      canvas.style("padding","10px")
      canvas.parent("p5-canvas-div")

      xMiddle = p5.width/2
      yMiddle = p5.height/2
      p5.frameRate(4)

      p5.background(color)
      // gridex
      for(let i = 0; i < p5.width; i += gridCellWidth){
        if(i != 0){
          p5.line(i,5,i,p5.height-5)
          p5.line(p5.width-5,i,5,i)
        }
      }
    }

    p5.draw = () => {
      
      // calculate next position
      let x = xMiddle + radius * Math.cos(timeStep)
      let y = yMiddle + radius * Math.sin(timeStep)
      
      // draw dot at position
      p5.ellipse(x,y,size,size)

      timeStep += speed;
    }

  })

  
}

main()