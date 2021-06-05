const functions = require('firebase-functions');
const bodyParser = require('body-parser');
const cookieParser = require('cookie-parser');
const cors = require('cors');
const express = require('express');
const apiRoutes = require('./api/routes');

const app = express();

const options = { origin: "http://localhost:3000", credentials: true };

app.use(cors(options));
app.use(cookieParser());
app.use(bodyParser.json());

const errorMiddleware = require('./api/middleware/errors');

app.use('/api', apiRoutes);
app.use(errorMiddleware);

exports.app = functions.https.onRequest(app);
