import axios from 'axios';
import PropTypes from 'prop-types';
import React, { createContext, useReducer, useContext } from 'react';

const initialState = { loggedIn: false, name: {} };
const store = createContext(initialState);

const { Provider } = store;

const BASE_URL = 'https://animal-colony-76d9b.firebaseapp.com/api';

const LOGIN = 'LOGIN';
const LOGOUT = 'LOGOUT';
const REGISTER = 'REGISTER';
const COLONY = 'COLONY';
const ANIMALS = 'ANIMALS';
const SORT = 'SORT';
const ALPHASORT = 'ALPHASORT';
const DELETE = 'DELETE';
const EDITANIMAL = 'EDITANIMAL';
const DELETEANIMAL = 'DELETEANIMAL';
const IMAGEUPLOAD = 'IMAGEUPLOAD';
const NOTE = 'NOTE';

axios.defaults.withCredentials = true;

const ProfileProvider = ({ children }) => {
  const [state, dispatch] = useReducer((prevState, action) => {
    const { type, payload } = action;

    switch (type) {
      case LOGIN: {
        // Store the profile data in the state
        console.log(payload);
        return { ...prevState, loggedIn: true, ...payload };
      }

      case REGISTER: {
        // Store registered profile in the state
        return { ...prevState, loggedIn: true, ...payload };
      }

      case COLONY: {
        const colonies = [payload, ...prevState.ownedColonies];
        return { ...prevState, ownedColonies: colonies };
      }

      case DELETE: {
        const colonies = payload.sharedTable ? [...prevState.sharedColonies] : [...prevState.ownedColonies];
        const newList = colonies.filter((item, index) => item.colonyId !== payload.colonyId);
        console.log('Deleted Colony ID: ', payload);
        return payload.sharedTable ? { ...prevState, sharedColonies: newList } : { ...prevState, ownedColonies: newList };
      }

      case DELETEANIMAL: {
        const animals = [...prevState.animals];
        const newList = animals.filter((item, index) => item.animalUUID !== payload);
        console.log('Deleted Animal ID: ', payload);
        return { ...prevState, animals: newList };
      }

      case SORT: {
        const colonies = [...prevState.ownedColonies];
        colonies.sort((a, b) => {
          if (a[payload] < b[payload]) {
            return -1; // locations swap
          }
          if (a[payload] > b[payload]) {
            return 1;
          }
          return 0;
        });
        return { ...prevState, ownedColonies: colonies };
      }

      case ALPHASORT: {
        const colonies = [...prevState.ownedColonies];
        colonies.sort((a, b) => {
          if (a[payload].toLowerCase() < b[payload].toLowerCase()) {
            return -1;
          }
          if (a[payload].toLowerCase() > b[payload].toLowerCase()) {
            return 1;
          }
          return 0;
        });
        return { ...prevState, ownedColonies: colonies };
      }

      case ANIMALS: {
        // Store colony animals in the state
        return {
          ...prevState, colonyId: payload.colonyId, accessRights: payload.accessRights, colonyName: payload.colonyName, colonySize: payload.colonySize, animals: payload.animals,
        };
      }

      case EDITANIMAL: {
        const animals = [...prevState.animals];
        const targetIndex = animals.findIndex(item => item.animalUUID === payload.animalUUID);
        console.log('here');
        // Get index of animal to edit
        if (targetIndex !== -1) {
          animals[targetIndex] = payload; // Store edited animal
        }
        return {
          ...prevState, animals,
        };
      }

      case IMAGEUPLOAD: {
        const animals = [...prevState.animals];
        const targetIndex = animals.findIndex(item => item.animalUUID === payload.animalId);
        console.log('here');
        // Get index of animal to edit
        if (targetIndex !== -1) {
          animals[targetIndex].imageLinks.push(payload.url); // Store edited animal
        }
        return {
          ...prevState, animals,
        };
      }

      case NOTE: {
        const animals = [...prevState.animals];
        const targetIndex = animals.findIndex(item => item.animalUUID === payload.animalId);
        console.log('here');
        // Get index of animal to edit
        if (targetIndex !== -1) {
          animals[targetIndex].notes.push(payload.note); // Store edited animal
        }
        return {
          ...prevState, animals,
        };
      }

      case LOGOUT: {
        // Reset state to logged out
        return initialState;
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
    .catch(function(response) {
      console.log(response.data.status);
    })
    .then(({ data }) => {
      dispatch({ type: LOGIN, payload: data });
    });

  const register = credentials => axios
    .post(`${BASE_URL}/user`, credentials)
    .then(({ data }) => {
      dispatch({ type: REGISTER, payload: data });
    });

  const logout = () => dispatch({
    type: LOGOUT,
  });

  const addColony = newColony => axios
    .post(`${BASE_URL}/colony`, newColony)
    .then(({ data }) => {
      dispatch({ type: COLONY, payload: data });
    });

  const shareColony = shareInfo => axios
    .post(`${BASE_URL}/colony/share`, shareInfo);

  const getAnimals = async (pageInfo, accessRights, colonyName, colonySize) => axios
    .post(`${BASE_URL}/animals`, pageInfo)
    .then(({ data }) => {
      data.accessRights = accessRights;
      data.colonyName = colonyName;
      data.colonySize = colonySize;
      dispatch({ type: ANIMALS, payload: data });
    });

  const deleteColony = (colonyId, sharedTable) => axios
    .post(`${BASE_URL}/colony/delete`, { colonyId }) // passing colony id to the colony id object
    .then(() => {
      dispatch({ type: DELETE, payload: { colonyId, sharedTable } });
    });

  const deleteAnimal = request => axios
    .post(`${BASE_URL}/animals/delete`, request)
    .then(() => {
      dispatch({ type: DELETEANIMAL, payload: request.animalId });
    });

  const editAnimal = request => axios
    .post(`${BASE_URL}/animals/edit`, request)
    .then(({ data }) => {
      dispatch({ type: EDITANIMAL, payload: data });
    });

  const storeImageLink = request => axios
    .post(`${BASE_URL}/animals/storeImageLink`, request)
    .then(({ data }) => {
      dispatch({ type: IMAGEUPLOAD, payload: data });
    });

  const storeNote = request => axios
    .post(`${BASE_URL}/animals/storeNote`, request)
    .then(({ data }) => {
      dispatch({ type: NOTE, payload: data });
    });


  const sortList = (sortBy) => {
    dispatch({ type: SORT, payload: sortBy });
  };

  const sortAlpha = (sortBy) => {
    dispatch({ type: ALPHASORT, payload: sortBy });
  };

  return {
    state,
    dispatch,
    login,
    logout,
    register,
    addColony,
    getAnimals,
    sortList,
    sortAlpha,
    deleteColony,
    deleteAnimal,
    editAnimal,
    shareColony,
    storeImageLink,
    storeNote,
  };
};

ProfileProvider.propTypes = {
  children: PropTypes.node.isRequired,
};

export { ProfileProvider, useProfileProvider };
