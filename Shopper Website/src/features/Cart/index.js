/* eslint-disable react/no-array-index-key */
/* eslint-disable no-unused-vars */
import React, { useState } from 'react';
import { useProfileProvider } from 'contexts/profile';
import Logout from 'components/Logout';

const Cart = () => {
  const { state, item, state: { item: itemsList, name: { first } } } = useProfileProvider();
  const [itemDetails, setItemDetails] = useState({ });

  // console.log("State item", JSON.stringify(itemsList.items));

  const listOfList = itemsList.items.map((object, i) => (
    <tr key={i}>
      <td>{object.name}</td>
      <td>{object.cost}</td>
    </tr>
  ));

  return (
    <div className="Cart">
      <h1>Shopping Cart</h1>
      <table style={{
 width: '100%', backgroundColor: 'darkgrey', color: 'white', textAlign: 'center',
}}
      >
        <tbody>
          <tr
            key="header"
            style={{
 width: '100%', backgroundColor: 'darkblue', color: 'white', textAlign: 'center',
}}
          >
            <th>Item</th>
            <th>Cost</th>
          </tr>
          {listOfList}
        </tbody>
      </table>
      <h2>Total Cost: {itemsList.totalCost}</h2>
    </div>
  );
};

export default Cart;
