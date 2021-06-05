const bcrypt = require('bcrypt');

const dataService = require('../services/database');
const jwt = require('../services/jwt');

/**
 * Returns the current authenticated user's details
 * @param req
 * @param res
 */
const currentUser = (req, res) => {
  const { user } = req;
  res.json(user);
};

/**
 * Attempts to create a user with the given registration details
 * @param req
 * @param res
 * @param next
 * @returns {Promise<void>}
 */
const createUser = async (req, res, next) => {
  console.log(req.body);
  const {
    body: {
      firstName, lastName, email, password,
    },
  } = req;
  if (!firstName || !lastName) {
    next(Error(`Missing ${!firstName ? 'first' : 'last'} name`));
  }

  const passwordHash = bcrypt.hashSync(password, 5);
  const ownedColonies = [];
  const sharedColonies = [];

  await dataService.createUser({
    name: {
      first: firstName,
      last: lastName,
      full: `${firstName} ${lastName}`,
    },
    email,
    ownedColonies,
    sharedColonies,
    passwordHash,
  }).then((userDetails) => {
    const { email } = userDetails;
    delete userDetails.passwordHash;
    const authToken = jwt.createToken({ email });
    res
      .cookie('session', authToken, { sameSite: 'none', secure: true })
      .status(200)
      .json(userDetails);
  }).catch((err) => {
    next(Error(`Unable to register user at this time: ${err.toString()}`));
  });
};

module.exports = { createUser, currentUser };
