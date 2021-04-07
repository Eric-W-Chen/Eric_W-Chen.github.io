/* eslint-disable no-unused-vars */
/* eslint-disable jsx-a11y/anchor-is-valid */
import React, { useEffect } from 'react';
import { useProfileProvider } from 'contexts/profile';
import Logout from 'components/Logout';
import { Link } from 'react-router-dom';

const Dashboard = () => {
  const { state, state: { name: { first } } } = useProfileProvider();

  return (
    <div className="dashboard">
      <h1>{`Welcome ${first}!`}</h1>
      {/* <h2>{ JSON.stringify(state) }</h2> */}
      <Link to="/item"> Item </Link>
      &nbsp;
      <Link to="/cart"> Shopping Cart </Link>
      &nbsp;
      <Logout />
    </div>
  );
};

export default Dashboard;
