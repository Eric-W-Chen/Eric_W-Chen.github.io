/* eslint-disable no-unused-vars */
/* eslint-disable jsx-a11y/anchor-is-valid */
import React from 'react';
import { useProfileProvider } from 'contexts/profile';
import Logout from 'components/Logout';
import LoginForm from 'components/LoginForm';
import { Link, Redirect } from 'react-router-dom';

const HomePage = () => {
  const { state, state: { loggedIn } } = useProfileProvider();
  return (
    <div className="home-page">
      <h1>Welcome to the HomePage, the future home of greatness!</h1>
      {/* <h2>{JSON.stringify(state)}</h2> */}
      {loggedIn ? <Logout /> : <LoginForm />}
      {loggedIn ? <Link to="/dashboard"> DashBoard</Link> : null}
      { loggedIn ? <Redirect to="/dashboard" /> : null }
    </div>
  );
};

export default HomePage;
