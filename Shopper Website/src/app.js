import React from 'react';
import { BrowserRouter as Router, Redirect, Route, Switch } from 'react-router-dom';
import { ProfileProvider, useProfileProvider } from 'contexts/profile';
import HomePage from 'features/HomePage';
import Dashboard from 'features/Dashboard';
import Register from 'components/Register';
import Item from 'features/Item';
import Cart from 'features/Cart';

/**
 * Renders a react-router enabled app with a wrapper to facilitate shared styles
 * and markup; add new routes for pages here.
 */
const App = () => (
  <Router>
    <ProfileProvider>
      <Switch>
        <ProtectedRoute path="/dashboard" component={Dashboard} />
        <ProtectedRoute path="/item" component={Item} />
        <ProtectedRoute path="/cart" component={Cart} />
        <Route exact path="/" component={HomePage} />
        <Route exact path="/register" component={Register} />
      </Switch>
    </ProfileProvider>
  </Router>
);


const ProtectedRoute = (props) => {
  const { state: { loggedIn } } = useProfileProvider();
  if (!loggedIn) return <Redirect to="/" />;

  return (<Route {...props} />);
};

export default App;
