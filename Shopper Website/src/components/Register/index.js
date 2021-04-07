/* eslint-disable no-unused-vars */
/* eslint-disable max-len */
import React, { useState } from 'react';
import { useProfileProvider } from 'contexts/profile';
import { BrowserRouter as Router, Redirect, Route, Switch, Link } from 'react-router-dom';

const Register = () => {
  const { register, state, state: { loggedIn } } = useProfileProvider(); const [userDetails, setUserDetails] = useState({ });

  const attemptRegister = (event) => {
    event.preventDefault();
    // console.log('attempt register', { userDetails });
    register(userDetails);
  };

  /**
   * A reusable function to update the state with a key/value pair where the
   * key is the name of the component and the value is its most recent value...
   *
   * This is a great pattern to use if you need to make the UI react to the input
   * in more complex forms and if you need the most recent value of the users'
   * submission before they click submit for validation purposes...
   * @param name
   * @param value
   */
  const updateInput = ({ target: { name, value } }) => {
    setUserDetails(prevState => ({ ...prevState, [name]: value }));
  };


  return (
    <div>
      { loggedIn ? <Redirect to="/dashboard" /> : null }
      <form className="Register">
        <p>
          <input name="firstName" placeholder="First Name" type="text" onChange={updateInput} />
        </p>
        <p>
          <input name="lastName" placeholder="Last Name" type="text" onChange={updateInput} />
        </p>
        <p>
          <input name="username" placeholder="Username" type="text" onChange={updateInput} />
        </p>
        <p>
          <input name="password" placeholder="Password" type="password" onChange={updateInput} />
        </p>
        <button type="submit" onClick={attemptRegister} onChange={updateInput}>
        Register
        </button>
      </form>
    </div>
  );
};

export default Register;
