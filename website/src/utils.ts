
// answer from, https://stackoverflow.com/questions/951021/what-is-the-javascript-version-of-sleep
export async function sleep(miliseconds:number = 2000){
  await new Promise(r => setTimeout(r,miliseconds)); // sleep
}

export interface MapUpdate {
  yaw:number
  xPos:number
  yPos:number
}