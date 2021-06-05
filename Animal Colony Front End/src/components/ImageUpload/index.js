import React, { useState } from 'react';
import { storage } from 'components/FirebaseConfig';
import { useProfileProvider } from 'contexts/profile';
import { Button, Dialog, DialogActions, DialogContent, DialogContentText, DialogTitle } from '@material-ui/core';
import CloudUploadIcon from '@material-ui/icons/CloudUpload';
import Add from '@material-ui/icons/Add';


const Uploader = (props) => {
  const [image, setImage] = useState(null);
  const [progress, setProgress] = useState(0);
  const [error, setError] = useState('');
  const [addDialog, setAddDialogOpen] = React.useState(false);
  const { state, storeImageLink } = useProfileProvider();
  const { colonyId } = state;
  const { animalId } = props;

  const handleChange = (e) => {
    const file = e.target.files[0];
    if (file) {
      const fileType = file.type;
      const validImageTypes = ['image/gif', 'image/jpeg', 'image/png'];
      if (validImageTypes.includes(fileType)) {
        setError('');
        setImage(file);
      } else {
        setError('Please select an image to upload');
      }
    }
  };

  const openAddDialog = () => {
    setAddDialogOpen(true);
  };

  const closeAddDialog = () => {
    setAddDialogOpen(false);
  };

  const handleUpdate = () => {
    closeAddDialog();
    if (image) {
      const uploadTask = storage.ref(`images/${image.name}`).put(image);

      uploadTask.on(
        'state_changed',
        (snapshot) => {
          const progress = Math.round((snapshot.bytesTransferred / snapshot.totalBytes) * 100);
          setProgress(progress);
        },
        (error) => {
          setError(error);
        },
        () => {
          storage
            .ref('images')
            .child(image.name)
            .getDownloadURL()
            .then((url) => {
              console.log('url', url);
              storeImageLink({ colonyId, animalId, url });
              setProgress(0);
            });
        },
      );
    } else {
      setError('Error please choose an image to upload');
    }
  };

  return (
    <div>
      <Button startIcon={<Add />} variant="outlined" onClick={openAddDialog}>
          Upload Image
      </Button>

      <Dialog open={addDialog} onClose={closeAddDialog} aria-labelledby="form-dialog-title">
        <DialogTitle id="form-dialog-title">Upload Image</DialogTitle>
        <DialogContent>
          <DialogContentText>
              Upload an image for this animal
          </DialogContentText>
          <input type="file" name="file" onChange={handleChange} />
        </DialogContent>
        <DialogActions>
          <Button onClick={handleUpdate} startIcon={<CloudUploadIcon />}>Upload</Button>
        </DialogActions>
      </Dialog>
      <div style={{ height: '100px' }}>
        {progress > 0 ? <progress value={progress} max="100" /> : ''}
        <p style={{ color: 'red' }}>{error}</p>
      </div>
    </div>
  );
};

export default Uploader;
