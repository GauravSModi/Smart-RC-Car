# Bootstrap
Yarn+TypeScript bootstrap referenced: [link](https://losikov.medium.com/part-1-project-initial-setup-typescript-node-js-31ba3aa7fbf1)  

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

```

```

# Building/Running
### Requirements:


---
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


---
### Running:
Custom yarn commands are already configured in `package.json`

`dev`: run ts-node on entry point during development(This should be the goto command for us), bypassing the compilation from TS to JS, but not recommend in releases.

`build`: build typscript files into javascripts.  
TS files from `src/` into JS files to `/bin/`

`start`: run the built javascripts in `/bin` via node. The entry point(the executed file) is specified in the commands under `"scripts"`

