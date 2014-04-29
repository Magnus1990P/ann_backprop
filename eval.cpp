//Written by Magnus Øverbø - 2014

//Includes
#include <stdio.h>				//Standard library for some things
#include <iostream>				//Standard input output cin/cout
#include <fstream>				//Used for file streams
#include <cstdlib>				//Used for rand()
#include <cmath>					//Used for math. abs(), exp()
#include <iomanip>
using namespace std;			//Namespace to use

// Global values
const int 	NUM_OF_LETTER		= 26;		//Number of different charachters in set
const int 	SETS_PER_LETTER	= 20;		//Number of different images per character
const int 	NUM_OF_PIXELS		= 100;	//Number of input values(pixels)
//const int 	NUM_OF_OUTPUTS	= 5;		//binary input
const int 	NUM_OF_OUTPUTS	= 26;		//char output
const int 	NUM_OF_LAYERS		= 1;		//Hidden layers
const int 	NUM_IN_HIDDEN		= 10;		//Number of nodes in each hidden layer
const float	LEARNING_RATE		= 0.7;	//The amount to learn for current change

class Node;					//Pre declaration of Node class

class Weight{
	private:
		int			id;			//	identifier
		Node*		to;			//	next layer the weight links to
		float		weight;	//	the current weight
		float		change;	//	the last delta change
		Weight*	next;		//	the next Weight element in list
	public:
		Weight(){};
		Weight( int, float, Node* );
		
		int			getId(){			return id;			};//Grab id
		float		getWeight(){	return weight;	};//Grab weight value
		Weight*	getNext(){		return next;		};//Grab next item
		Node*		getNode(){		return to;			};//Grab the linked Node object
		
		void		setNext( Weight* );
	
		void		display();
		bool 		idEqual( int );
};

class Node{
	private:
		int 		id;					//	the identifier
		float		input;			//	the input value as decimal
		float		output;			//	the output value as decimal
		float		error;			//	the current error of the node as decimal
		Weight*	weight;			//	the link to the Weight list to next layer
		Node* 	next;				//	next Node in the list
		bool		convergence;//	If the Node is within the treshold
	public:
		Node( ){};
		Node( int );
		Node( int, Weight* );
	
		int			getId(){					return id;					};//Grab id
		float		getInput(){				return input;				};//Grab input value
		float		getOutput(){			return output;			};//Grab output value
		Node*		getNext(){		 		return next; 				};//Grab next item

		void		setInput();
		void		setInput( float );
		void		setWeights( Weight* );
		void		setNext( Node* );
		
		void		display();
		void		updateInput( float );
		void		calcOutput();
		void		pushForward();
		void		reset();
		void		fornicate();
};

class Data{
	private:
		char	fasit;		//	The identifying type of the Data
		int		*target;	//	Int array of 0/1 values representing the target
		float *values;	//	float array of values representing the 100 pixels
		Data*	next;			//	link to next Data element in the list

	public:
		Data(){};
		Data( char tF, int *tT, float *tV );

		Data* getNext(){ 					return next;			};//Grab next element
		float	getValue( int i ){	return values[i]; };//Grab a specific pixel value
		char  getFasit(){ 				return fasit;			};//Get the id type

		void 	setNext( Data* );

		void 	display();
		void 	loadData( char, int );
		int*	getTarget(){	return target;	}
};

Node*		input;									//Ptr to input layer list
Node*		hidden[NUM_OF_LAYERS];	//Array of ptrs to all hidden layers
Node*		output;									//Ptr to the output layer list
Data* 	dHead;									//Ptr to the head of Data list
int*		curTarget;							//Ptr to the array specifying the current target
Data*		inData;									//Ptr to the current input data element

void		loadDataSet();							//Load dataset into Data objects from file
void		networkSetup();							//Sets up a feed forward full connected ANN
Weight* genWeights( int, Node*, ifstream& );		//Returns a list of len lengt
void 		findInput( char, int );			//Push a set of input to the network
void		display();									//Display all about all nodes
void		clearInput();								//Clear input and outputs
void		pushForward();							//Calc output and add to the connected layer
void 		displayOutput();						//Display output
void		displayDataSet();						//Display all data sets
void		displayTarget();						//displays the target output
void		classify( int, char );			//

int main(){
	char testChar = 'A';							//Initial image to start on
	char endChar 	= 'Z';							//Which image to end on
	int  testSet	= 0;								//Initial imageset to start on
	int	 endSet		= 20;								//Which imageset to end on
	int  count		= 0;								//Counter for which round is running
	bool quit			= false;						//quit checker
	loadDataSet();										//Load input data
	networkSetup();										//Set up the network with links

	cout << ",    ";
	for( int i=0; i<NUM_OF_LETTER; i++){
		cout << "," << right	<< setw(5)	<< char( int('Z')-i );
	}
	cout << "\n";

	do{
		if( testChar == endChar && testSet == endSet-1 )
			quit = true;
		findInput( testChar, testSet );		//Select inputdata to input-layer
		input->setInput();								//Push the inputs to the input layer
		pushForward();										//Propagate the input through the network
		classify(	testSet, testChar );		//Classify the data in RAW/percentages

		if( testSet == endSet-1 ){					//If end of current character set 
			cout << "AVG, " << testChar << ", \n\n"; 					//print to skips for CSV
			if( !quit ){
				cout << " ,   ";								//Print letter IDs on top
				for( int i=0; i<NUM_OF_LETTER; i++){
					cout << "," << right	<< setw(5)	<< char( int('Z')-i );
				}
				cout << "\n";
			}
			
			testChar=char(int(testChar)+1);	//Grab the next character set
			testSet		= 0;									//Reset counter
		}																	//End if check for last character of set
		else{															//If not last character of set
			testSet++;											//increment testSet
		}																	//End of else

		clearInput();											//Clear the values
	}while( !quit );
//	}while( testChar != endChar || testChar == endChar && testSet != endSet );//Run the tests for all data set
	
	return 0;														//Exit gracefully
}


/**
	*	\author	20140416 - Magnus Øverbø	
	*	\brief	Prints/classifies the output of the output nodes in percent or
	*	decimals
	**/
void classify( int i, char c ){
	Data*	cData = inData;			//Temp ptr for holding current element of dataset
	Node*	cNode	= output;			//Temp ptr for holding the output list
														//Print out info about the data set
	cout<< right << setw(3)	<< i << ", " << c;
	while( cNode != NULL ){		//Loop through the output layer
																												//print actual value
		//cout << "," << right	<< setw(10)	<< cNode->getOutput();
																												//print value in percent
		cout << "," << right	<< setw(4)	<< int(cNode->getOutput() * 100) << "%";
		cNode = cNode->getNext();														//go to next node
	}													//end of loop
	cout << "\n";							//print new line
}

/**
	*	\author	20140416 - Magnus Øverbø	
	* \brief 	Set input values of the Node list
	*
	*	Set the input variable to its pixel value. Then calculate the input value to
	*	a relative value between -1 and +1, using a linear function. Then do the
	*	same for the next element.
	**/
void	Node::setInput(){
	input		= inData->getValue( id );			//Set input to real pixel value 0-255
	output	= (input > 200) ? 0 : 1;			//Calculate the value to a relative value
	if( next != NULL )	next->setInput();	//If next is a object call setInput
}

/**
	*	\author  20140416 - Magnus Øverbø	
	* \brief 	Sets the input and output var to the paramater
	**/
void	Node::setInput( float f){
	input	= f;
	output= f;
}

/** \author	20140416 - Magnus Øverbø
	*	\param	Float		delta value to add to the existing input value
	* \brief	Updates the input value of the node by adding the parameter to
	* current value.
	**/
void	Node::updateInput( float add ){
	input += add;									//append value to input	
};

/** \author	20140416 - Magnus Øverbø
	* \brief	Pushes each nodes output value to its links
	*
	*	For the current node go through the entire weight list. For each weight temp
	*	store the end Node. For the end node update its input with the Current nodes
	*	output value multiplied by the links weight. Then move to the next weight in
	*	the list.  Finally do the same for the next node in the layer
	**/
void	Node::pushForward(){
	Weight* curEl	= weight;				//Temp ptr for the weight list
	Node*		curNode;							//Temp ptr for the end node of current weight

	while( curEl != NULL ){				//For each weight in the weight list
		curNode = curEl->getNode();	//Grab the end Node for current link
																//Update the input of linked node
		curNode->updateInput( output * curEl->getWeight() );
		curEl = curEl->getNext();		//Grab the next element in the list
	}															//End of Weight list loop
																//If next is not null do the same for it
	if(next != NULL)	next->pushForward();//do the same for next node i layer
}

/** \author	20140416 - Magnus Øverbø	
	* \brief	Calculates the output for the nodes
	*
	*	Calculates the output for the entire layer using the Sigmoid function
	*	1/(1+e^(-net)). Then if the next ptr is not null do the same for this
	**/
void	Node::calcOutput(){
	output = 1 / ( 1 + exp( -input ) );			//calculate output with sigmoid
	if( next != NULL )	next->calcOutput();	//move to next and calc its output
};

/** \author	20140416 - Magnus Øverbø	
	* \brief	Pushes the input values through the network
	*
	*	It pushes the values through the network. First it pushes the input layer
	*	values through to the first hidden layer. THen it for each hidden layer
	*	calculates the output value. Followed by pushing the calculated output
	*	values to the next layer.  Finally it calculates the output valuies for the
	*	output layer.
	**/
void pushForward(){
	input->pushForward();				//Pushes the outputs of input layer to the next
	
	for( int i=0; i<NUM_OF_LAYERS; i++){//For all hidden layers
		hidden[i]->calcOutput();					//Calculate output values for layer
		hidden[i]->pushForward();					//Push output values to next layer
	}																		//End of hidden layer loop

	output->calcOutput();								//Calculate output values for output
	output->pushForward();							//Pushes output layer forward
}


/** \author	20140416 - Magnus Øverbø	
	* \brief	Display values of the output targets
	**/
void	displayTarget(){
	for( int i=0; i<NUM_OF_OUTPUTS; i++)//For all ints between 0 and global var
		cout << curTarget[ i ] << " ";		//Print target int followed by space
}

/** \author	20140416 - Magnus Øverbø	
	* \brief	Displays the inforation about the weight link
	*	
	*	Prints the weights id number, the linked nodes ID, the value of the weight,
	*	the nodes input value and the nodes output value.  Then it moves to the
	*	next weight in the list and repeats the process
	**/
void Weight::display(){
	cout << "\n     "	<< right	<<	setw(3)		<<	id 							//weight id
			 << " [ "			<< right	<<	setw(3)		<<	to->getId() 		//linked nodes id
			 << "   " 		<< left		<<	setw(11)	<<	weight					//weight value
			 << "   " 		<< left		<<	setw(11)	<<	to->getInput()	//linked node output value
			 << "   " 		<< left		<<	setw(11)	<<	to->getOutput()	//linked node output value
			 << " ]";
	if( next != NULL )	next->display();	//go to next weight and print it
}

/** \author	20140416 - Magnus Øverbø	
	* \brief	Display the Node information
	*
	*	Prints the node id, WEIGHT, input value, output value, calculated error, the
	*	absolute difference between target and output followed by whether it has
	*	converged. Then it prints all the weights. Followed by displaying the next
	*	element in the list
	**/
void Node::display(){
	cout << ""					<< left 	<< setw(3) 	<< id						//Id
			 << "IN:"				<< right	<< setw(11)	<<	input				//Input
			 << "   OUT:" 	<< right	<< setw(13)	<<	output	 		//output
			 << "   ERR:"		<< right	<< setw(13)	<<	error				//error
			 																											//offset from target
			 << "   OFF:" 	<< right	<< setw(13)	<<	curTarget[ id ] - output
			 << "   CON: "	<< ((convergence) ? "true":"false");	//Converged or not
	if( weight != NULL ){																		//Informative message
		cout << "\n     ### [ ###   weight        input         output      ]";
		weight->display();	//prints the weights
	}
	cout << "\n";
	if( next 	 != NULL )	next->display();		//If next display it
}

/** \author	20140416 - Magnus Øverbø	
	* \brief	Displays all layers and their information
	**/
void	display(){
	input->display();							//print input layer
	cout << "\n\n";
	for( int i=0; i< NUM_OF_LAYERS; i++ ){//For all hidden layers
		hidden[i]->display();				//print hidden layer
		cout << "\n\n";
	}
	output->display();						//Print output layer
	cout << "\n\n";
	displayTarget();							//Display the current target
	cout << "\n\n";
}


/** \author	20140416 - Magnus Øverbø	
	* \brief	Displays the target and pixel values in the data set
	**/
void Data::display(){
	cout << fasit << ":\t[";
	for( int i=0; i<NUM_OF_OUTPUTS; i++)	cout << target[i] << " ";
		cout << "]\n\t[";
	for( int i=0; i<NUM_OF_PIXELS; i++)	cout << values[i] << " ";
		cout << "]\n";
}

/** \author	20140416 - Magnus Øverbø	
	* \brief	Displays all data sets
	**/
void displayDataSet(){
	Data* el = dHead;				//temp ptr to hold the current data set
	while( el != NULL ){		//For all sets in the data set
		el->display();				//Display	the current set
		el = el->getNext();		//grab next element in set
	}												//end of data set loop
}


/** \author	20140416 - Magnus Øverbø	
	* \brief	Clones the input value over to the output value
	**/
void	Node::fornicate(){
	output = input;
};

/** \author	20140416 - Magnus Øverbø	
	* \brief	Resets/Zeroes out all Nodes in the layer.
	*
	*	Resets input, output, error and convergence 
	**/
void	Node::reset(){
	input		= 0;
	output	= 0;
	error		= 0;
	convergence = false;
	if( next != NULL ) next->reset();
};

/** \author	20140416 - Magnus Øverbø	
	* \brief	Clears the non-persistant data in all layers
	**/
void clearInput(){
	input->reset();						//Reset the input layer
	for( int i=0; i<NUM_OF_LAYERS; i++)//For all hidden layers
		hidden[i]->reset();			//Reset the hidden layer
	output->reset();					//Reset the output layers
}

/** \author	20140416 - Magnus Øverbø	
	* \brief	Find data set to use as input and target
	**/
void	findInput( char c, int i){
	bool	found		= false;				//Stopper for while loop
	int		count		=	0;						//Counter to decide which to select
	Data* curData = dHead;				//Temp ptr to hold the current dataSet
	while( curData != NULL && found != true){	//For all data sets
		if( curData->getFasit() == c ){					//If desired character set
																						//If correct set is found
			if( count == i || count == SETS_PER_LETTER-1 ){
				found	 = true;											//Set found to true
				inData = curData;										//Set inData ptr to current set
				curTarget = curData->getTarget( );	//Set target ptr to current target
			}																			//End found correct dataset
			count++;											//increment counter
		}																//end character found if
		curData = curData->getNext( );	//move to next data set
	}//End of data set loop
}



/** \author	20140416 - Magnus Øverbø	
	*	\param	int					Lenght of the list
	*	\param	Node*				First node in layer which it links to
	*	\param	ifstream\&	Filestream to read weights from
	*	\retval	Weight*	Returns first real element in the list of weights
	* \brief	Generates a list of weights of length len
	*
	*	It creates a list of Weight starting with a foo element, later dropped. Then
	*	it creates a random weight value which is used to create the new weight
	*	object. Then it adds it to the list. THis is repeated until the limit is met
	*	and the second object of list is returned to the caller. Thus removing the
	*	first foo element.
	**/
Weight* genWeights( int len, Node* list, ifstream &in){
	float 	weight;									//Temp var to hold the generated weight
	Node*		curNode = list;					//Temp ptr to hold the list of Nodes
	Weight* wStar 	= new Weight();	//Temp var to hold the start of the list
	Weight*	tWeight = wStar;				//Temp ptr to hold last element in list
	Weight* nWeight;								//Temp ptr to create new Weight objects

	for(int i=0; i<len; i++){				//For as long as specified
		weight = 0;
		in >> weight; in.ignore();
		nWeight = new Weight( i, weight, curNode);	//Create new Weight object
		tWeight->setNext( nWeight );	//Put the new Weight as the next el in list
		tWeight = tWeight->getNext();	//Make tWeight point to the next Weigt in list
		curNode = curNode->getNext();	//Make curNode point to the next Node in list
	}
	return wStar->getNext();				//Return the second element in list to remove
																	//	the foo element used as first element
}

/** \author	20140416 - Magnus Øverbø	
	* \brief	Sets up the initial network with initial weights
	*
	*	Creates first the input layer followed by each of the hidden layers. Then
	*	finally the output layer by creating and linking foo elements in chains
	*	after their respective head elements.  Then, for layer except the output
	*	layer, set the weight lists of each node, by creating the weight list tha
	*	links to the next layer.
	**/
void networkSetup(){
	Node	*tNode, *nNode;		//Temp ptrs for holding the element and new instances
	output	= new Node();		//Create foo element for output ptr
	input 	= new Node();		//Create foo element for input ptr
	tNode = input;					//Set tNode to point to input layer
	int pixels, outputs, layers, nodes;

	ifstream in;
	in.open("savefile2.net");
	if( in.is_open() ){
		in >> pixels; 	in.ignore();
		in >> outputs;	in.ignore();
		in >> layers; 	in.ignore();
		in >> nodes; 		in.ignore();
		
		for(int i=0; i<pixels; i++){				//For all nodes in input layer
			nNode	= new Node( i );						//Create new node with auto incremented id
			tNode->setNext( nNode );					//Set the next ptr to the new instance
			tNode = tNode->getNext();					//Move tNode to the new instance
		}																		//End of for loop
		input		= input->getNext();					//Drop first foo element in list
		
		for(int i=0; i<layers; i++){				//For all hidden layers
			hidden[i] = new Node();						//Create first dummy node
			tNode = hidden[i];								//Set temp ptr to the first node in layer
			for(int j=0; j<nodes; j++){				//For all noeds in layer
				nNode	= new Node( j );					//Creat new instance of Node
				tNode->setNext( nNode );				//Set next ptr to the new instance
				tNode = tNode->getNext();				//Move temp ptr to the new instance
			}																	//End of for loop
			hidden[i] = hidden[i]->getNext();	//Drop first foo element in list
		}																		//End of for loop

		tNode		= output;										//Set temp ptr to the output layer
		for(int i=0; i<outputs; i++){				//For all nodes in output layer
			nNode	= new Node( i );						//Create new instance of Node object
			tNode->setNext( nNode );					//Set the next ptr to the new instance 
			tNode = tNode->getNext();					//Move temp ptr to the next item in list
		}																		//End of output layer loop
		output	= output->getNext();				//Drop first foo element in list

		//////////////		WEIGHT CREATION			/////////////////
		tNode = input;											//Set temp ptr to input layer
		while( tNode != NULL ){							//for all Nodes in input layer
			tNode->setWeights( genWeights( nodes, hidden[0], in ) );
			tNode = tNode->getNext();					//Move to next Node in input layer
		}																		//End of input layer loop

		for(int i=0; i<layers; i++){				//For all hidden layers
			tNode = hidden[i];								//Set temp ptr to point to current layer
	
			while( tNode != NULL ){						//For all nodes in the hidden layer
				if( i+1 == layers )							//If last hidden layer(link to output layer)
					tNode->setWeights( genWeights( outputs, output, in) );
				else														//If not last layer( link to next layer)
					tNode->setWeights( genWeights( nodes, hidden[i+1], in ) );
				tNode = tNode->getNext();				//Move temp ptr to the next item in list
			}																	//End of hidden layer loop
		}																		//End of loop for all hidden layers
	}																			//End of file loop
}																				//end of function

/** \author	20140416 - Magnus Øverbø	
	*	\brief	Loads all sets of images into memory
	*
	*	For each image set(A-Z) it reads the answer key(A-Z), and target values.
	*	Then it reads for all 20 instances their input values and adds them to a
	*	list in memory.  First element of the list is dHead, which Holds image
	*	A1.jpg
	**/
void loadDataSet(){
	dHead = new Data();		//Create a foo object for the head of the list
	Data* dTemp = dHead;	//Create and set dTemp to point to the head of the list
	Data* dNew;						//Create a new Data ptr, used to create new objects
	char	tFas;						//Temp var for the character the dataset depicts
	int		*tTarg;					//Int ptr for holding the array of target values
	float	*tVals;					//float ptr for holding the array of input values

	ifstream in;						//Create and open the filestream for the file with
	in.open("./data2.dta");	//	data from the python script

	in >> tFas;	in.ignore();//Read the first answer character of file
	while( !in.eof() ){												//Read file while not end of file
		
		tTarg = new int[NUM_OF_OUTPUTS];				//Create int array in the tTarg ptr
		for( int i=0; i<NUM_OF_OUTPUTS; i++ ){	//For all target values read them
			in >> tTarg[i];		in.ignore();				//	into the array from file
		}

		for( int i=0; i<SETS_PER_LETTER; i++ ){	//For each set of images in anaswer
			tVals = new float[NUM_OF_PIXELS];			//Create new float array for values
			for( int i=0; i<NUM_OF_PIXELS; i++ ){	//For all pixel values, read them 
				in >> tVals[i];		in.ignore();			//	into the new array
			}

			dNew = new Data( tFas, tTarg, tVals );//Create a new object for dNew with
																						//	the char, target and values
			dTemp->setNext( dNew );								//Set next value of dTemp to dNew
			dTemp = dTemp->getNext();							//Set dTemp to point to its new next
		}																				//End of image sets
		in >> tFas;	in.ignore();								//Red the answer key
	}																					//End of file loop
	dHead = dHead->getNext();									//Drop the dummy element in dHead
																						//	by moving dHead to its neighbor
}


/** \author	20140416 - Magnus Øverbø	
	*	\brief	Constructor for the Data.
	*
	*	It set the answer(fasit), the target ptr to point to an array of int values,
	*	the values ptr to point to an array of floats which represents the pixel
	*	values. THe next ptr is zeroed out
	**/
Data::Data( char tF, int *tT, float *tV ){
	fasit		= tF;
	target	= tT;
	values	= tV;
	next		= NULL;
}

/** \author	20140416 - Magnus Øverbø	
	* \brief	Constructor for Node.
	*
	* It sets the id as specified and zeroes out all other values. THis is useful
	* for creating the network without links, and then create the links afterward.
	* Specially when creating from input to output.
	**/
Node::Node( int tI ){
	id	 		= tI;			//integer id
	weight	= NULL;
	input		= 0;
	output	= 0;
	error		= 0;
	next		= NULL;
}

/** \author	20140416 - Magnus Øverbø	
	* \brief	Constructor for the Node.
	*
	*	It sets id and the list of weights automatically. Could be used when
	*	creating the network from output to input. It zeroes out all other
	*	variables.
	**/
Node::Node( int tI, Weight* tW ){
	id	 		= tI;		//integer id
	weight	= tW;		//list of weights
	input		= 0;
	output	= 0;
	error		= 0;
	next		= NULL;
}

/** \author	20140416 - Magnus Øverbø	
	* \brief	Constructor for the Weight.
	*
	*	sets id, Node link and weight. Then set change and next to 0 and NULL.
	**/
Weight::Weight( int tI, float tW, Node* tN){
		id		=	tI;
		to		= tN;
		weight=	tW;
		change= 0.0;
		next	= NULL;
}


/** \author	20140416 - Magnus Øverbø	
	* \brief	Set the next ptr to point to the specified Data item
	**/
void 	Data::setNext( Data* tData ){
	next = tData;
}		

/** \author	20140416 - Magnus Øverbø	
	* \brief	Set the weight ptr to point to the first element of a Weight list
	**/
void	Node::setWeights( Weight* tW ){
	weight = tW;
}

/** \author	20140416 - Magnus Øverbø
	* \brief	Set the next ptr to point to the specified Node item
	**/
void	Node::setNext( Node* tNode ){
	next = tNode;
}

/** \author	20140416 - Magnus Øverbø	
	* \brief	Set the next ptr to point to the specified Weight item
	**/
void	Weight::setNext( Weight* tW){
	next = tW;
}
		
