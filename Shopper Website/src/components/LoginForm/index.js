/* eslint-disable jsx-a11y/anchor-is-valid */
/* eslint-disable no-unused-vars */
import React, { useState } from 'react';
import { useProfileProvider } from 'contexts/profile';
import { BrowserRouter as Router, Redirect, Route, Switch, Link } from 'react-router-dom';

const Login = () => {
  const { login } = useProfileProvider();
  const [userDetails, setUserDetails] = useState({ });

  const attemptLogin = (event) => {
    event.preventDefault();
    login(userDetails);
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
      <Link to="/register">Register</Link>
      <form className="login-form">
        <input name="username" placeholder="Username" type="text" onChange={updateInput} />
        <input name="password" placeholder="Password" type="password" onChange={updateInput} />
        <button type="submit" onClick={attemptLogin} onChange={updateInput}>
        Login
        </button>
      </form>
    </div>
  );
};

export default Login;
