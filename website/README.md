# Website Module

A website for user interaction with rc.

# Building/Running

### **Requirements:**
`yarn` (package manager):  
For unix. Similar function as "apt-get" in terms of linux, but for node modules
```
sudo apt remove cmdtest
sudo apt remove yarn
curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add -
echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list
sudo apt-get update
sudo apt-get install yarn -y
```

---
`tsc` (node typescript compiler):  
Used for compiling typescript files into javascript to be ran by node.js, and to be published.
```
sudo apt install node-typescript
```

## **Pre-Running:**

___Only need to be done for the first time:___ 

Go to website directory (assuming you are at project root) and **download required yarn packages**
```
cd ./website
yarn # this command download required packages for the first time
```

## **Running**
### **Format:**
Custom yarn commands are already configured in `package.json`

```
# general format
yarn <command name>

# example
yarn dev
```

### **On Host:**

`dev`: run ts-node on host during development directly on typscripts. Doesn't built javascript files (This should be the goto command for us)

`start`: run the built javascripts in `website/build/` via node. The entry point(the executed file) is specified in `package.json`

`build`: runs `MakeFile` that  
1. Builds typscript files into javascripts. TS files from `website/src/` into JS files to `website/build/`
2. Create `~/cmpt433/public/project-web/` if directory doesn't exist
3. copy everything in `website/build` and `website/run-scripts` to `~cmpt/public/project-web`

## **On target (BGG)**
running `startNode.sh` starts node on `app.js`

`public/project-web/` structure:
```
.
├── startNode.sh
├── app.js # entry point
└── <other typescript files in src/>.js
└── 
```
*note not sure how htmls fits in this yet



# TS development Examples
`src/apps.ts`: Sample entry point file 
```

// Defining a function
function sampleMain(){

  // console.log is bascially the same as printf/cout in node.js
  console.log()
} 

// javascript(typescript) is like python, it runs everything in a file sequentially, thus
console.log('Hello World!!')
// call function
sampleMain()
```
`src/simpleUDPserver.ts` Sample UDP server
```
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
```

`src/simpleUDPclient.ts` Sample UDP client

```
/**
 * A simple udp socket client
 */

 import dgram from "dgram"

 export function simpleUDPclient(){
  const client = dgram.createSocket("udp4")
  // send message with parameters
  // send(message, offset, length, port, address)
  client.send("Hi Server!",0,10,12345,"localhost",()=>{

    // log after message is sent
    console.log("message is sent!")

    // close client after sending one message
    client.close()
  })
}
```

# Troubleshooting Node
If you're having trouble with packages, a node.js upgrade might fix it for ya! (example bugs include below) [reference](https://www.esparkinfo.com/how-to-update-node-js-to-latest-version.html)   
\*note: This procedure installs `nvm` (node version manager) in the process, which should be the easiest way to update/change version of node.

0. check current version (latest version at the time of writing is v17.5.0)
```
node --version
```

1. do an update and install a few dependency
```
sudo apt update
sudo apt install build-essential checkinstall libssl-dev
```
2. execute install script for nvm over internet
```
curl -o- https://raw.githubusercontent.com/creationix/nvm/v0.35.1/install.sh | bash
```
3. install latest version
```
nvm install node
```
4. \* restart the terminal

5. check version again
```
node --version
```


# About Bootstrap
Yarn+TypeScript bootstrap referenced: [link](https://losikov.medium.com/part-1-project-initial-setup-typescript-node-js-31ba3aa7fbf1)  

