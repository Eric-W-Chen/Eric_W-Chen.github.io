import firebase from 'firebase';
import 'firebase/storage';

const firebaseConfig = {
  apiKey: 'AIzaSyCv8FAn2HZzu1RAa6-EwSHOqgiyhr9ZlGs',
  authDomain: 'animal-colony-76d9b.firebaseapp.com',
  databaseURL: 'https://animal-colony-76d9b.firebaseio.com',
  projectId: 'animal-colony-76d9b',
  storageBucket: 'animal-colony-76d9b.appspot.com',
  messagingSenderId: '289026487338',
  appId: '1:289026487338:web:e294ae051c2c75ab6e3f53',
};
// Initialize Firebase
firebase.initializeApp(firebaseConfig);

const storage = firebase.storage();

export { storage, firebase as default };
