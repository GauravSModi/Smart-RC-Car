// yarn + typscript bootstrap https://losikov.medium.com/part-1-project-initial-setup-typescript-node-js-31ba3aa7fbf1

import * as customNameForUDPserver from "./simpleUDPserver"
import * as client from "./simpleUDPclient"

// Defining a function
function sampleMain(){

  console.log("welcome to main.")
  customNameForUDPserver.simpleUDPserver()

  client.simpleUDPclient()
} 

// javascript(typescript) is like python, it runs everything in a file sequentially, thus

// console.log is bascially the same as printf/cout in node.js
console.log('Hello World!!')
// call function
sampleMain()
