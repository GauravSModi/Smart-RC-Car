# Website Module

A website for user interaction with rc.

# Building/Running
use shell script in project root `compileNode.sh`

## **Running**
### **Format:**
Custom npm commands are already configured in `package.json`

```
# general format
npm run <command name>

# example
npm run dev
```

### **On Host:**

`dev`: `nodemon -e ts --exec \"npm run build && npm run start\"`
runs development mode directly locally and automatically reloads on any file save. Variation: `dev-win` for windows.

`start`: `node ./build/httpServer.js`
run the built javascripts in `website/build/` via node. The entry point(the executed file) is specified in `package.json`

`build`: runs `tsc && npm run copy-assets && npm run browerify`
1. Builds typscript files into javascripts. TS files from `website/src/` into JS files to `website/build/`
2. copy everything in `website/build` and `website/run-scripts` to `~cmpt/public/project-web`
3. bundle all dependecies for node client code into single javascript file to serve

## **On target (BGG)**
running `startNode.sh` starts node on `app.js`


# Troubleshooting Node
If you're having trouble with packages, a node.js upgrade might fix it for ya! (example bugs include below) [reference](https://www.esparkinfo.com/how-to-update-node-js-to-latest-version.html)   
## First Method
follow guide by Brian Fraser

## Second Method
Only if first methods doesn't work. Not recommended
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

