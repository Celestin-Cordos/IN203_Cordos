#include <iostream>
# include <chrono >
# include <random >
# include <mpi.h>
// Attention , ne marche qu'en C++ 11 ou supérieur :

using namespace std;



	typedef std:: chrono :: high_resolution_clock myclock;
	myclock :: time_point beginning = myclock ::now();
	myclock :: duration d = myclock ::now() - beginning;
	unsigned seed = d.count ();
	std:: default_random_engine generator(seed);
	std:: uniform_real_distribution <double > distribution ( -1.0 ,1.0);
	unsigned long nbDarts = 0;
	// Throw nbSamples darts in the unit square [ -1:1] x [ -1:1]
	for ( unsigned sample = 0; sample < nbSamples; ++ sample ) 
	{
		double x = distribution(generator);
		double y = distribution(generator);
		// Test if the dart is in the unit disk
		if ( x*x+y*y<=1 ) nbDarts ++;
	}
	// Number of nbDarts throwed in the unit disk
	double ratio = double(nbDarts)/double(nbSamples);
	return ratio;

int main( int nargs, char* argv[] )
{
	// On initialise le contexte MPI qui va s'occuper :
	//    1. Créer un communicateur global, COMM_WORLD qui permet de gérer
	//       et assurer la cohésion de l'ensemble des processus créés par MPI;
	//    2. d'attribuer à chaque processus un identifiant ( entier ) unique pour
	//       le communicateur COMM_WORLD
	//    3. etc...
	MPI_Init( &nargs, &argv );
	// Pour des raisons de portabilité qui débordent largement du cadre
	// de ce cours, on préfère toujours cloner le communicateur global
	// MPI_COMM_WORLD qui gère l'ensemble des processus lancés par MPI.
	MPI_Comm globComm;
	MPI_Comm_dup(MPI_COMM_WORLD, &globComm);
	// On interroge le communicateur global pour connaître le nombre de processus
	// qui ont été lancés par l'utilisateur :
	int nbp;
	MPI_Comm_size(globComm, &nbp);
	// On interroge le communicateur global pour connaître l'identifiant qui
	// m'a été attribué ( en tant que processus ). Cet identifiant est compris
	// entre 0 et nbp-1 ( nbp étant le nombre de processus qui ont été lancés par
	// l'utilisateur )
	int rank;
	MPI_Comm_rank(globComm, &rank);
	// Création d'un fichier pour ma propre sortie en écriture :int myval = 3;
	MPI_Status status;


	unsigned nbDartsTransfert = 0;
	unsigned nbSamples = 1000; // pour chaque processus

	if (rank == 0)
	{
		unsigned nbDartsGlobal = 0;
		for (int i = 1; i< nbp)
		{
			MPI_Recv (&nbDartsTransfert, 1, MPI_UNSIGNED, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			nbDartsGlobal += nbDartsTransfert;
		}
		float r = nbDartsGlobal/((nbp-1)*nbSamples);
		cout << "Pi est environ egal à : " << 4*r << endl;
	}
	else 
	{
		typedef std:: chrono :: high_resolution_clock myclock;
		myclock :: time_point beginning = myclock ::now();
		myclock :: duration d = myclock ::now() - beginning;
		unsigned seed = d.count ();
		std:: default_random_engine generator(seed);
		std:: uniform_real_distribution <double > distribution ( -1.0 ,1.0);
		unsigned nbDarts = 0;
		// Throw nbSamples darts in the unit square [ -1:1] x [ -1:1]
		for ( unsigned sample = 0; sample < unsigned (nbSamples/(nbp-1)) + 1; ++ sample ) 
		{
			double x = distribution(generator);
			double y = distribution(generator);
			// Test if the dart is in the unit disk
			if ( x*x+y*y<=1 ) nbDarts ++;
		}
		nbDartsTransfert = nbDarts;
		MPI_Send (&nbDartsTransfert, 1, MPI_UNSIGNED, 0, rank, MPI_COMM_WORLD );
		// Number of nbDarts throwed in the unit disk
		
	}
	
	
	MPI_Finalize();
	return EXIT_SUCCESS;
}