cd ./website
npm run build
rsync -av --exclude='README.md' ./ ~/cmpt433/public/project-web/