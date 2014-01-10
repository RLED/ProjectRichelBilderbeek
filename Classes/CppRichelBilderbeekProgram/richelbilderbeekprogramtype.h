//---------------------------------------------------------------------------
/*
RichelBilderbeek::ProgramType, enum for a program by Richel Bilderbeek
Copyright (C 2013-2014 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/ProjectRichelBilderbeek.htm
//---------------------------------------------------------------------------
#ifndef RICHELBILDERBEEKPROGRAMTYPE_H
#define RICHELBILDERBEEKPROGRAMTYPE_H

#include <vector>

namespace ribi {

///ProgramTypes works with ProgramType
enum class ProgramType
{
  aminoAcidFighter,
  athleticLand,
  athleticLandVcl,
  asciiArter,
  asciiArterVcl,
  barbaImage,
  beerWanter,
  beerWanterVcl,
  bochum,
  boenken,
  boenkenVcl,
  brainweaver,
  bristol,
  chrisWiley,
  codeToHtml,
  connectThree,
  corridor,
  createGlossary,
  createQtProjectZipFile,
  crossPoll,
  dasWahreSchlagerfest,
  dasWahreSchlagerfestVcl,
  dotMatrix,
  dotMatrixVcl,
  encranger,
  everythingToPiecesShooter,
  fakeEvy,
  filterOperationer,
  filterOperationerVcl,
  fryskLeareLieder,
  functionPlotter,
  functionPlotterVcl,
  gaborFilter,
  gaborFilterVcl,
  grayCoder,
  grayCoderVcl,
  gtst,
  histogramEqualizationer,
  histogramEqualizationerVcl,
  hometrainer,
  hometrainerVcl,
  imageRotater,
  imageRotaterClx,
  imageRotaterVcl,
  k3OpEenRij,
  k3OpEenRijVcl,
  kalmanFilterer,
  keySender,
  knokfighter,
  knokfighterVcl,
  kTouchLectureCreator,
  lambdaBot,
  learyCircumplex,
  logisticGrowthSimulator,
  loose,
  martianCafeTuinemaTycoon,
  mazeCreator,
  mazeCreatorVcl,
  maziak,
  maziakVcl,
  metZnDrieen,
  metZnDrieenVcl,
  midiLessonCreator,
  morpher,
  multiEncranger,
  multiEncrangerVcl,
  muscaDomestica,
  musicTheory,
  ndsmake,
  ndsPaint,
  paperRockScissors,
  pause,
  perfectElasticCollision,
  picToCode,
  pixelator,
  pixelatorVcl,
  pokeVolley,
  pong,
  preDickAdvocaTor,
  primeExpert,
  projectRichelBilderbeek,
  pylos,
  qmakeWatcher,
  quadraticSolver,
  rampalEtienne,
  randomCode,
  randomCodeVcl,
  rasper,
  refrigeratorPuzzleSolver,
  regexTester,
  reversi,
  richelBilderbeekGallery,
  richelbilderbeekNlSitemapGenerator,
  rubiksClock,
  rubiksClockVcl,
  searchAndDestroyChess,
  searchAndDestroyChessVcl,
  secretMessage,
  secretMessageVcl,
  simBrainiac,
  simImmuneResponse,
  simMysteryMachine,
  simplifyNewick,
  simPredator,
  simStagecraft,
  soaSim,
  solvePuzzleX,
  spaceHarry,
  spaceHarryVcl,
  staircaseCardCreator,
  stateObserver,
  styleSheetSetter,
  superNsanaBros,
  surfacePlotter,
  surfacePlotterVcl,
  tankBattalion,
  testAbout,
  testApproximator,
  testBinaryNewickVector,
  testBouncingBallsWidget,
  testBouncingRectsWidget,
  testBroadcastServer,
  testChess,
  testConceptMap,
  testDial,
  testDrawCanvas,
  testEntrance,
  testExercise,
  testFunctionParser,
  testGnuplotInterface,
  testGraphicsProxyWidget,
  testGravityWidget,
  testGroupWidget,
  testHugeVector,
  testImageCanvas,
  testLazy_init,
  testLed,
  testManyDigitNewick,
  testMultiApproximator,
  testMultipleChoiceQuestion,
  testMultiVector,
  testNdsmake,
  testNeuralNet,
  testNewick,
  testNewickVector,
  testOpenQuestion,
  testPylos,
  testQrcFile,
  testQtArrowItems,
  testQtCreatorProFile,
  testQtHideAndShowDialog,
  testQtKeyboardFriendlyGraphicsView,
  testQtModels,
  testQtOcrWidget,
  testQtRoundedEditRectItem,
  testQtRoundedRectItem,
  testQtRoundedTextRectItem,
  testQuestion,
  testReversi,
  testSelectFileDialog,
  testServerPusher,
  testShape,
  testShinyButton,
  testSimpleLinearRegression,
  testStopwatch,
  testTextCanvas,
  testTextPositionWidget,
  testTicTacToe,
  testTimedServerPusher,
  testToggleButton,
  testTwoDigitNewick,
  thorVeen,
  thresholdFilterer,
  thresholdFiltererVcl,
  ticTacToe,
  ticTacToeLearner,
  ticTacToeValuer,
  timePoll,
  tronCollection,
  tronCollectionVcl,
  ubuntuOneWatcher,
  vanDenBogaart,
  virtualBastard,
  visualAbc,
  xeNonZero,
  zork,

  n_types //Used for debugging
};

} //~namespace ribi

#endif // RICHELBILDERBEEKPROGRAMTYPE_H
