/* eslint-disable no-unused-vars */
import React, { useState } from 'react';
import { useProfileProvider } from 'contexts/profile';
import Logout from 'components/Logout';

const Item = () => {
  const { state, item, state: { name: { first } } } = useProfileProvider();
  const [itemDetails, setItemDetails] = useState({ });

  const attemptSubmit = (event) => {
    event.preventDefault();
    // console.log('attempt submit', { itemDetails });
    item(itemDetails);
  };

  const updateInput = ({ target: { name, value } }) => {
    setItemDetails(prevState => ({ ...prevState, [name]: value }));
  };

  return (
    <div className="dashboard">
      <h1>{`Welcome ${first}!`}</h1>
      <h2>{JSON.stringify(state) }</h2>
      <Logout />

      <form className="Item">
        <p>
          <input name="name" placeholder="name" type="text" onChange={updateInput} />
        </p>
        <p>
          <input name="cost" placeholder="cost" type="number" onChange={updateInput} />
        </p>
        <button type="submit" onClick={attemptSubmit} onChange={updateInput}>
      Submit
        </button>
      </form>
    </div>
  );
};

export default Item;
