import React, { useState } from 'react';
import { useProfileProvider } from 'contexts/profile';
import { Button, Container, CssBaseline, CardHeader, Table, TableBody, TableCell, TableContainer, TableRow, Paper, Avatar } from '@material-ui/core';
import Modal from '@material-ui/core/Modal';
import Card from '@material-ui/core/Card';
import CardContent from '@material-ui/core/CardContent';
import Typography from '@material-ui/core/Typography';
import FirstPageIcon from '@material-ui/icons/FirstPage';
import KeyboardArrowLeft from '@material-ui/icons/KeyboardArrowLeft';
import KeyboardArrowRight from '@material-ui/icons/KeyboardArrowRight';
import LastPageIcon from '@material-ui/icons/LastPage';
import { blue, red } from '@material-ui/core/colors';
import { makeStyles, useTheme } from '@material-ui/core/styles';
import TableFooter from '@material-ui/core/TableFooter';
import TablePagination from '@material-ui/core/TablePagination';

import IconButton from '@material-ui/core/IconButton';
import DeleteIcon from '@material-ui/icons/Delete';
import { Redirect } from 'react-router-dom';

const paginationStyle = makeStyles(theme => ({
  root: {
    flexShrink: 0,
    marginLeft: theme.spacing(2.5),
  },
}));

function TablePaginationActions(props) {
  const classes = paginationStyle();
  const theme = useTheme();
  const {
    page, count, rowsPerPage, onChangePage,
  } = props;

  const handleFirstPageButtonClick = (event) => {
    onChangePage(event, 0);
  };

  const handleBackButtonClick = (event) => {
    onChangePage(event, page - 1);
  };

  const handleNextButtonClick = (event) => {
    onChangePage(event, page + 1);
  };

  const handleLastPageButtonClick = (event) => {
    onChangePage(event, Math.max(0, Math.ceil(count / rowsPerPage) - 1));
  };

  return (
    <div className={classes.root}>
      <IconButton
        onClick={handleFirstPageButtonClick}
        disabled={page === 0}
        aria-label="first page"
      >
        {theme.direction === 'rtl' ? <LastPageIcon /> : <FirstPageIcon />}
      </IconButton>
      <IconButton onClick={handleBackButtonClick} disabled={page === 0} aria-label="previous page">
        {theme.direction === 'rtl' ? <KeyboardArrowRight /> : <KeyboardArrowLeft />}
      </IconButton>
      <IconButton
        onClick={handleNextButtonClick}
        disabled={page >= Math.ceil(count / rowsPerPage) - 1}
        aria-label="next page"
      >
        {theme.direction === 'rtl' ? <KeyboardArrowLeft /> : <KeyboardArrowRight />}
      </IconButton>
      <IconButton
        onClick={handleLastPageButtonClick}
        disabled={page >= Math.ceil(count / rowsPerPage) - 1}
        aria-label="last page"
      >
        {theme.direction === 'rtl' ? <FirstPageIcon /> : <LastPageIcon />}
      </IconButton>
    </div>
  );
}


const useStyles = makeStyles(theme => ({
  table: {
    width: '100%',
    minWidth: 500,
    marginTop: 8,
  },
  paper: {
    position: 'absolute',
    width: '60%',
    padding: theme.spacing(2, 4, 3),
    outline: 0,
  },
  root: {
    display: 'flex',
    flexDirection: 'column',
  },
  details: {
    display: 'flex',
    flex: 1,
    flexDirection: 'column',
  },
  content: {
    display: 'flex',
    flex: '1',
    flexDirection: 'row',
  },
  cover: {
    width: 151,
  },
  controls: {
    display: 'flex',
    alignItems: 'right',
    alignContent: 'right',
    alignSelf: 'flex-end',
    paddingLeft: theme.spacing(1),
    paddingRight: theme.spacing(1),
    paddingBottom: theme.spacing(1),
  },
  playIcon: {
    height: 38,
    width: 38,
  },
  blue: {
    color: theme.palette.getContrastText(blue[800]),
    backgroundColor: blue[300],
  },
  red: {
    color: theme.palette.getContrastText(red[800]),
    backgroundColor: red[300],
  },
}));


const Animals = () => {
  const classes = useStyles();
  const [page, setPage] = React.useState(0);
  const rowsPerPage = 10;
  const [openModal, setOpenModal] = React.useState(false);
  const [currentAnimal, setCurrentAnimal] = useState({});
  const [redirectToDetails, setRedirectTodetails] = useState(false);
  const { state, getAnimals, deleteAnimal } = useProfileProvider();
  const {
    animals, accessRights, colonyId, colonySize, colonyName,
  } = state;

  const permissions = accessRights ? 'Read and Write' : 'Read Only';
  console.log('access', accessRights);

  const handleChangePage = async (event, newPage) => {
    const request = {
      colonyId, rowsPerPage, page: newPage,
    };
    await getAnimals(request, accessRights, colonyName, colonySize);
    setPage(newPage);
  };

  const handleOpenModal = (animal) => {
    setCurrentAnimal(animal);
    setOpenModal(true);
  };

  const handleCloseModal = () => {
    setOpenModal(false);
  };

  const deleteChosenAnimal = async (animalId) => {
    const request = {
      colonyId, animalId,
    };
    await deleteAnimal(request);
  };

  if (redirectToDetails) {
    return (<Redirect
      // to={`/animal/${currentAnimal.mouseId}`}
      to={{
        pathname: `/animal/${currentAnimal.mouseId}`,
        state: { animal: currentAnimal },
      }}
    />);
  }


  return (
    <Container component="main" style={{ padding: 8 }}>
      <CssBaseline />
      <h1>Colony: {colonyName}</h1>
      <h2>Access:{permissions}</h2>

      <TableContainer className={classes.table} component={Paper}>
        <Table className={classes.table} aria-label="custom pagination table">
          <TableBody>
            <TableRow>
              <TableCell style={{ borderRight: '1px solid rgba(224, 224, 224, 1)' }}>ID</TableCell>
              <TableCell align="right" style={{ borderRight: '1px solid rgba(224, 224, 224, 1)' }}>Gender</TableCell>
              <TableCell align="right" style={{ borderRight: '1px solid rgba(224, 224, 224, 1)' }}>Litter</TableCell>
              <TableCell align="right" style={{ borderRight: '1px solid rgba(224, 224, 224, 1)' }}>Father&nbsp;ID</TableCell>
              <TableCell align="right" style={{ borderRight: '1px solid rgba(224, 224, 224, 1)' }}>Mother&nbsp;ID</TableCell>
              <TableCell align="right" style={{ borderRight: '1px solid rgba(224, 224, 224, 1)' }}>Date&nbsp; of&nbsp; Birth</TableCell>
              <TableCell align="right" style={{ borderRight: '1px solid rgba(224, 224, 224, 1)' }} />
            </TableRow>
          </TableBody>
          <TableBody>
            {(animals
            ).map(animal => (
              <TableRow key={animal.mouseId}>
                <TableCell
                  style={{ cursor: 'pointer', borderRight: '1px solid rgba(224, 224, 224, 1)' }}
                  component="th"
                  scope="row"
                  onClick={() => {
                    handleOpenModal(animal);
                  }}
                >
                  <div style={{ fontWeight: 'bold', fontSize: 18, flexDirection: 'row' }}>
                    <Avatar className={animal.gender === 'M' ? classes.blue : classes.red}>{animal.mouseId}</Avatar>
                  </div>
                </TableCell>
                <TableCell align="right" style={{ borderRight: '1px solid rgba(224, 224, 224, 1)' }}>
                  {animal.gender}
                </TableCell>
                <TableCell align="right" style={{ borderRight: '1px solid rgba(224, 224, 224, 1)' }}>
                  {animal.litter}
                </TableCell>
                <TableCell align="right" style={{ borderRight: '1px solid rgba(224, 224, 224, 1)' }}>
                  {animal.fatherId}
                </TableCell>
                <TableCell align="right" style={{ borderRight: '1px solid rgba(224, 224, 224, 1)' }}>
                  {animal.motherId}
                </TableCell>
                <TableCell align="right" style={{ borderRight: '1px solid rgba(224, 224, 224, 1)' }}>
                  <span>{animal.dobMonth}/{animal.dobDay}/{animal.dobYear}</span>
                </TableCell>
                <TableCell align="center" style={{ borderRight: '1px solid rgba(224, 224, 224, 1)' }}>
                  <Button
                    variant="outlined"
                    color="primary"
                    onClick={() => {
                      setCurrentAnimal(animal);
                      setRedirectTodetails(true);
                    }}
                  >Profile
                  </Button>
                  {
                    accessRights ?
                      <IconButton
                        aria-label="delete"
                        className={classes.margin}
                        onClick={() => {
                          if (accessRights) {
                            deleteChosenAnimal(animal.animalUUID);
                          } else {
                            console.log('User does not have write access');
                          }
                        }}
                      >
                        <DeleteIcon />
                      </IconButton>
                      : null
                  }

                </TableCell>
              </TableRow>
            ))}
          </TableBody>

          <TableFooter>
            <TableRow>
              <TablePagination
                rowsPerPageOptions={[]}
                colSpan={3}
                count={colonySize}
                rowsPerPage={10}
                page={page}
                SelectProps={{
                  inputProps: { 'aria-label': 'rows per page' },
                  native: true,
                }}
                onChangePage={handleChangePage}
                ActionsComponent={TablePaginationActions}
              />
            </TableRow>
          </TableFooter>
        </Table>
      </TableContainer>

      <Modal
        aria-labelledby="simple-modal-title"
        aria-describedby="simple-modal-description"
        open={openModal}
        onClose={handleCloseModal}
      >
        <div style={{ top: '20%', left: '20%' }} className={classes.paper}>

          <Card className={classes.root}>

            <div>
              <CardHeader
                avatar={
                  <Avatar alt={currentAnimal.mouseId} />
                }
                title={
                  <Typography gutterBottom variant="h5" component="h2">
                    ID: {currentAnimal.mouseId}
                  </Typography>
                }
                subheader={`Notes: ${currentAnimal.notes}`}
              />
            </div>
            <div className={classes.content}>
              <CardContent className={classes.details}>
                <Typography variant="subtitle1" color="textSecondary">
                  <strong>Gender:</strong> {currentAnimal.gender}
                </Typography>
                <Typography variant="subtitle1" color="textSecondary">
                  <strong>Litter:</strong> {currentAnimal.litter}
                </Typography>
                <Typography variant="subtitle1" color="textSecondary">
                  <strong>Father ID:</strong> {currentAnimal.fatherId}
                </Typography>
                <Typography variant="subtitle1" color="textSecondary">
                  <strong>Mother ID:</strong> {currentAnimal.motherId}
                </Typography>
                <Typography variant="subtitle1" color="textSecondary">
                  <strong>DOB Month:</strong> {currentAnimal.dobMonth}
                </Typography>
                <Typography variant="subtitle1" color="textSecondary">
                  <strong>DOB Day:</strong> {currentAnimal.dobDay}
                </Typography>
                <Typography variant="subtitle1" color="textSecondary">
                  <strong>DOB Year</strong> {currentAnimal.dobYear}
                </Typography>
              </CardContent>
              <CardContent className={classes.details}>
                <Typography variant="subtitle1" color="textSecondary">
                  <strong>DOD Month:</strong> {currentAnimal.dodMonth}
                </Typography>
                <Typography variant="subtitle1" color="textSecondary">
                  <strong>DOD Day:</strong> {currentAnimal.dodDay}
                </Typography>
                <Typography variant="subtitle1" color="textSecondary">
                  <strong>DOD Year</strong> {currentAnimal.dodYear}
                </Typography>
                <Typography variant="subtitle1" color="textSecondary">
                  <strong>Gene 1:</strong> {currentAnimal.gene1}
                </Typography>
                <Typography variant="subtitle1" color="textSecondary">
                  <strong>Gene 2:</strong> {currentAnimal.gene2}
                </Typography>
                <Typography variant="subtitle1" color="textSecondary">
                  <strong>Gene 3:</strong> {currentAnimal.gene3}
                </Typography>
                <Typography variant="subtitle1" color="textSecondary">
                  <strong>TOD:</strong> {currentAnimal.tod}
                </Typography>
              </CardContent>
            </div>

            <div className={classes.details}>
              <div className={classes.controls}>
                <Button variant="outlined" color="primary" onClick={handleCloseModal}>Done</Button>
              </div>
            </div>
          </Card>

        </div>
      </Modal>

    </Container >
  );
};

export default Animals;
