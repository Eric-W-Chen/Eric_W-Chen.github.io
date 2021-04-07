/* eslint-disable no-unused-vars */
/* eslint-disable no-use-before-define */
import axios from 'axios';
import PropTypes from 'prop-types';
import React, { createContext, useReducer, useContext } from 'react';

const initialState = {
  loggedIn: false, name: {}, item: null, itemsList: null,
};
const store = createContext(initialState);
const { Provider } = store;

const BASE_URL = 'http://localhost:5000/api';

const LOGIN = 'LOGIN';
const LOGOUT = 'LOGOUT';
const ITEM = 'ITEM';
const ITEMSLIST = 'ITEMSLIST';

axios.defaults.withCredentials = true;
axios.defaults.headers = {
  'Access-Control-Allow-Origin': '*',
  'Access-Control-Allow-Methods': 'GET, POST, DELETE, PUT, PATCH',
  'Access-Control-Allow-Headers': 'Origin, Content-Type, X-Auth-Token',
};


const ProfileProvider = ({ children }) => {
  const [state, dispatch] = useReducer((prevState, action) => {
    const { type, payload } = action;

    switch (type) {
      case LOGIN: {
        // Store the profile data in the state
        return { ...prevState, loggedIn: true, ...payload };
      }
      case LOGOUT: {
        // Reset state to logged out
        return initialState;
      }
      case ITEM: {
        return { ...prevState, item: payload };
      }
      case ITEMSLIST: {
        return { ...prevState, item: payload };
      }
      default:
        throw new Error();
    }
  }, initialState);

  return <Provider value={{ state, dispatch }}>{children}</Provider>;
};

const useProfileProvider = () => {
  const { state, dispatch } = useContext(store);

  const login = credentials => axios
    .post(`${BASE_URL}/login`, credentials)
    .then(({ data }) => {
      dispatch({ type: LOGIN, payload: data });
      getItems();
    });

  const register = credentials => axios
    .post(`${BASE_URL}/user`, credentials)
    .then(({ data }) => {
      dispatch({ type: LOGIN, payload: data });
    });

  const item = credentials => axios
    .post(`${BASE_URL}/cart`, { items: [credentials] })
    .then(({ data }) => {
      dispatch({ type: ITEM, payload: data });
    });

  const getItems = credentials => axios
    .get(`${BASE_URL}/cart`)
    .then(({ data }) => {
      dispatch({ type: ITEMSLIST, payload: data });
    });

  const logout = () => dispatch({
    type: LOGOUT,
  });

  return {
    state,
    dispatch,
    login,
    logout,
    register,
    item,
  };
};

ProfileProvider.propTypes = {
  children: PropTypes.node.isRequired,
};

export { ProfileProvider, useProfileProvider };
