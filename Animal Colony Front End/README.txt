
Animal Colony Management System Front End

Development Setup
You'll first need Node.js installed, however I recommend installing it through nodenv as it provides a great way to switch between projects' node versions automatically and may come in handy with your own sponsors' projects.

Installing Nodenv
The easiest way to install nodenv is through HomeBrew.

brew install nodenv
and then place the following into your ~/.bash_profile, ~/.bashrc or whatever file your shell sources:

eval "$(nodenv init -)"
If you have any questions, see the nodenv installation guide.

You'll then want to install the node version listed in .node-version at the root of this project:

cd animal-colony-frontend
nodenv install
Installing the Required Dependencies
Node.js comes pre-installed with NPM, and we use it to sync packages for the project.

To install the dependencies do

cd colony-frontend
npm install
Starting the App
You'll notice there are scripts in the package.json file, these are shorthand commands that you can also define to help accomplish tasks that you run frequently.

You can start up the app with

npm start
and you'll notice the app begins to listen on localhost:3000 and has hot-reloading built in! You won't need to manually start up the app every time you make a change, what a world we live in...
