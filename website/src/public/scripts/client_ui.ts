
import { RemoteCarControl } from './client_model'
import P5 from "p5"
import { MapUpdate } from '../../utils'

// Get reference to DOM elements
// shutdown Button
const shutdownButton = document.getElementById("shutdown-button") as HTMLInputElement

// controls button
const leftButton = document.getElementById("keyboard_key_left") as HTMLButtonElement
const rightButton = document.getElementById("keyboard_key_right") as HTMLButtonElement
const upButton = document.getElementById("keyboard_key_up") as HTMLButtonElement
const downButton = document.getElementById("keyboard_key_down") as HTMLButtonElement


let mapUpdateList:MapUpdate[] = []

mapUpdateList.push({
  xPos:0.0,
  yPos:0.0,
  yaw:0.0
})

function drawArrow(p5:P5,_x:number,_y:number,_angle:number){
  // referenced https://p5js.org/reference/#/p5.Vector/magSq
  p5.push(); // saves current drawing state
  
  const myColor = "black"
  const width = 3
  const ArrowLength = 20
  
  const radians = _angle * Math.PI / 180

  p5.stroke(myColor);
  p5.strokeWeight(width);
  p5.fill(myColor);
  p5.translate(_x, _y);
  p5.line(0, 0, ArrowLength * Math.cos(radians), ArrowLength * Math.sin(radians));
  //rotate(vec.heading());
  //translate(vec.mag() - arrowSize, 0);
  //triangle(0, arrowSize / 2, 0, -arrowSize / 2, arrowSize, 0);

  p5.pop(); // restore pushed drawing state
}

async function main(){
  RemoteCarControl.initialize(document)
  
  RemoteCarControl.connect()
  //notifyUpdateUI()
  //alert("JS loaded successfully YAY")
  //statusText.innerHTML = "Device up for:\n" + await BeatBox.getUpTime()

  leftButton.onclick = () => {
    console.log("left")
    RemoteCarControl.moveLeft()
  }

  rightButton.onclick = () => {
    console.log("right")
    RemoteCarControl.moveRight()
  }

  upButton.onclick = () => {
    console.log("up")
    RemoteCarControl.moveFront()
  }

  downButton.onclick = () => {
    console.log("down")
    RemoteCarControl.moveBack()
  }


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
      p5.frameRate(1)
    }

    p5.draw = () => {
      
      // calculate next position
      //let x = xMiddle + radius * Math.cos(timeStep)
      //let y = yMiddle + radius * Math.sin(timeStep)

      // clear background
      p5.background(color)
      // gridex
      for(let i = 0; i < p5.width; i += gridCellWidth){
        if(i != 0){
          p5.line(i,5,i,p5.height-5)
          p5.line(p5.width-5,i,5,i)
        }
      }

      // draw map points
      let newMapInfo:MapUpdate = {
        xPos:0.0,
        yPos:0.0,
        yaw:0.0
      }
      for(let i = 0; i < mapUpdateList.length;i++){
        let mapInfo = mapUpdateList[i]
        
        let x = xMiddle + mapInfo.xPos
        let y = yMiddle + mapInfo.yPos
        let yaw = mapInfo.yaw

        // draw dot at position
        p5.ellipse(x,y,size,size)

        if(i == mapUpdateList.length - 1){
          // only draw arrow
          // for last datapoint in queue
          drawArrow(p5,x,y,yaw);  

          newMapInfo.xPos = mapInfo.xPos + radius / 10
          newMapInfo.yPos = mapInfo.yPos + radius / 40
          newMapInfo.yaw = mapInfo.yaw = 4.0
          //console.log("x:"+x+",y:"+y+",yaw:"+yaw)
        }      
      }

      //mapUpdateList.push(newMapInfo)
    
      timeStep += speed;
    }

  })
}

export function addMapUpdate(newMapUpdate:MapUpdate){

  console.log("New MapUpdate added:")
  console.log("x:"+newMapUpdate.xPos+",y:"+newMapUpdate.yPos+",yaw:"+newMapUpdate.yaw)
  mapUpdateList.push(newMapUpdate)
}

main()