## Purpose
This application is an example of an app with sufficient state management, decoupling
of network calls, and how React hooks and functional components work.

## Goals

 Name                 | Description                                                                                                                                                                                                                                                                                                                                                           |
----------------------|-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
Add RegistrationForm | Create a registration form that unauthorized users can use to create an account with.<br><br>It should post a network request to the backend route `POST /api/users` with the following schema:<br><br>`{ firstName: string, lastName: string, username: string }`<br><br><br>It should update the state with the authenticated details and redirect to `/dashboard`  |
Add ItemForm         | Create a form that a user can fill out in order to add items to their shopping cart.<br><br>It should post a network request to the backend route `POST /api/cart`, with the cookie from<br>authentication, with the following or similar schema:<br><br>`{ name: string (i.e. 'iPhone X'), cost: Number (i.e. 250) }`                                                |
Cart Page            | Create a page that pulls from the endpoint `GET /api/cart`, with the cookie from<br>authentication, and draws them as a list with the names and costs.                                                                                                                                                                                                                |

## Development Setup
You'll first need [Node.js](https://nodejs.org/) installed, I recommend
installing it through [nodenv](https://github.com/nodenv/nodenv) as it provides
a great way to switch between projects' node versions automatically.

### Installing Nodenv
The easiest way to install nodenv is through [HomeBrew](https://brew.sh/).
```bash
brew install nodenv
```

and then place the following into your `~/.bash_profile`, `~/.bashrc` or
whatever file your shell sources:
```bash
eval "$(nodenv init -)"
```

If you have any questions, see the [nodenv installation guide](https://github.com/nodenv/nodenv).

You'll then want to install the node version listed in `.node-version` at the
root of this project:
```bash
cd ReactShopper-Client
nodenv install
```

### Installing the Required Dependencies
Node.js comes pre-installed with [NPM](https://www.npmjs.com), and we
use it to sync packages for the project.

To install the dependencies do
```bash
cd ReactShopper-Client
npm install
```

### Starting the App
You'll notice there are `scripts` in the `package.json` file, these
are shorthand commands that you can also define to help accomplish
tasks that you run frequently.

You can start up the app with
```bash
npm start
```

and you'll notice the app begins to listen on [localhost:3000](http://localhost:3000)
and has hot-reloading built in!  You won't need to manually start up the app
every time you make a change, what a world we live in...

### Configuring the Backend
The backend is stored as a separate project, and will need to be started
in a separate shell.  Find the setup instructions on the
[Shopper-API](https://github.com/USFClassrooms/Shopper-API) project page.
