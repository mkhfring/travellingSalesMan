#define MASTER	0

#include <mpi.h>
#include <vector>
#include <assert.h>
#include <algorithm>
#include <set>
#include <iomanip>

using namespace std;

const int MAXN = 16;
const int INF = 1e9;
const int MIN_EDGE_WEIGHT = 1;
const int MAX_EDGE_WEIGHT = 10;

long long fact[MAXN+1];

int random(int l, int r)
{
  return l + rand()%(r-l+1);
}

void precompute_factorial()
{
	fact[0] = 1;

	for(int i=1;i<=MAXN;i++)
	{
		fact[i] = i * fact[i-1];
	}
}

void assign_edge_weights(int** matrix, int N)
{
	for (int i = 0 ; i < N ; i++) {
		for (int j = i+1 ; j < N ; j++) {
			matrix[i][j] = random(MIN_EDGE_WEIGHT,MAX_EDGE_WEIGHT);
			matrix[j][i] = matrix[i][j];
		}
		matrix[i][i] = 0;
	}
}

vector<int> nth_permutation(vector<int>&arr, long long n)
{
	int N = arr.size();

	assert(n<=fact[N]);

	sort(arr.begin(),arr.end());
	
	set<int>st;
	for(int x : arr)st.insert(x);

	vector<int>ans;

	for(int i=0;i<N;i++)
	{
		int cn = 1;
		long long cval = fact[N-1-i];
		while(cval<n)
		{
			cn++;
			cval=(long long)cn*cval;
			cval=(long long)cval/(cn-1);
		}

		long long pval = cval*(cn-1)/cn;
		n -= pval;
		
		auto it = st.begin();
		for(int i=0;i<cn-1;i++)it++;
		ans.push_back(*it);
		st.erase(it);
	}

	return ans;
}

int find_path_cost(int** matrix, vector<int>&arr)
{
	int cost = 0;
	for(int i=1;i<(int)arr.size();i++)
	{
		cost += matrix[arr[i]][arr[i-1]];
	}
	return cost;
}

void print_matrix(int** matrix, int N)
{
	for(int i=0;i<N;i++)
	{
		for(int j=0;j<N;j++)
		{
			cout<<matrix[i][j]<<" ";
		}
		cout<<endl;
	}
}

int main(int argc, char *argv[])
{
	int N = stoi(argv[1]);

	int			num_proc;				/* Number of processors */
	int			my_rank;				/* current processor rank  */
	int			stat;					/* Error Status  */
	MPI_Status	status;

	double	start,end;					/* timing */

	MPI_Init(&argc, &argv);

	MPI_Comm_size(MPI_COMM_WORLD, &num_proc );
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank );

	precompute_factorial();
	
	int** matrix = new int*[N];
	for (int i = 0; i < N; i++) {
		matrix[i] = new int[N];
	}
	
	// assigning edge weights in MASTER
	if(my_rank == MASTER){
		assign_edge_weights(matrix, N);
	}
	// sending the edge weights to other PEs
	if(my_rank == MASTER){
		for(int proc = 1 ; proc < num_proc ; pe++) {
			for (int i = 0 ; i < N ; i++) {
				MPI_Send(&matrix[i][0], N, MPI_INT, pe, pe, MPI_COMM_WORLD);
			}
		}
	}
	else{
		for (int i = 0 ; i < N ; i++) {
			MPI_Recv( &matrix[i][0], N, MPI_INT, MASTER, my_rank, MPI_COMM_WORLD, &status);
		}
	}

	MPI_Barrier( MPI_COMM_WORLD );

	// printing the path weight matrix
	if(my_rank == MASTER){
		print_matrix(matrix, N);
		cout<<endl;
	}

	MPI_Barrier( MPI_COMM_WORLD );

	start = MPI_Wtime();			// start time

	// tsp parallel

	int optimal_value = INF;
	int* ans = new int[N+1];

	// divide among PEs

	long long nperm_proc = fact[N-1]/num_proc;
	long long rem = fact[N-1]%num_proc;

	long long start_perm_ind, end_perm_ind;


	std::vector<int>nodes;
	for(int i=1;i<N;i++) nodes.push_back(i);

	if(rem == 0){
		start_perm_ind = (my_rank*nperm_proc) + 1;
		end_perm_ind = (my_rank + 1)*nperm_proc;
	}
	else{
		if(my_rank < rem){
			start_perm_ind = (my_rank*(nperm_proc + 1)) + 1;
			end_perm_ind = (my_rank + 1)*(nperm_proc + 1);
		}
		else{
			start_perm_ind = rem*(nperm_proc + 1) + (my_rank - rem)*nperm_proc + 1;
			end_perm_ind = rem*(nperm_proc + 1) + (my_rank + 1 - rem)*nperm_proc;
		}
	}

	// int iter = 0;

	// compute only when some work is assigned to the current PE
	if(start_perm_ind <= end_perm_ind){
		vector<int> my_ans;

		vector<int>nodes_begin = nth_permutation(nodes, start_perm_ind);
		vector<int>nodes_end = nth_permutation(nodes, end_perm_ind);
		do
		{
			vector<int>temp = nodes_begin;
			temp.push_back(0);
			temp.insert(temp.begin(),0);
			int val = find_path_cost(matrix, temp);
			if(val < optimal_value)
			{
				optimal_value = val;
				my_ans = temp;
			}

			// iter++;
			if(nodes_begin == nodes_end) break;

		}while(next_permutation(nodes_begin.begin(),nodes_begin.end()));
		copy(my_ans.begin(), my_ans.end(), ans);

	}

	// comparing the optimal values from other PEs in the MASTER
	if(my_rank == MASTER){

		// receiving ans from all other PEs
		for(int proc = 1 ; proc < num_proc ; pe++) {

			int tmp_optimal_value;
			int* tmp_ans = new int[N+1];
			MPI_Recv(&tmp_ans[0], (N+1), MPI_INT, pe, pe, MPI_COMM_WORLD, &status);
			MPI_Recv(&tmp_optimal_value, 1, MPI_INT, pe, pe, MPI_COMM_WORLD, &status);

			if(tmp_optimal_value<optimal_value){
				optimal_value = tmp_optimal_value;
				copy(tmp_ans, tmp_ans+N+1, ans);
			}
		}
	}
	else{
		// send ans to MASTER
		MPI_Send(&ans[0], (N+1), MPI_INT, MASTER, my_rank, MPI_COMM_WORLD);
		MPI_Send(&optimal_value, 1, MPI_INT, MASTER, my_rank, MPI_COMM_WORLD);
	}

	MPI_Barrier( MPI_COMM_WORLD );

	end = MPI_Wtime();		// end time
	
	// printing the minimum cost path
	if(my_rank == MASTER){
		for (int i = 0; i < N+1; i++) {
			cout << ans[i] << ' ';
		}
		cout<<endl<<endl;
	}

	// printing the minimum path cost
	if(my_rank == MASTER){
		int cost = 0;
		for(int i=1;i<N+1;i++)
		{
			cost += matrix[ans[i]][ans[i-1]];
		}
		cout<<cost<<endl<<endl;
	}

	// printing the run-time
	if(my_rank == MASTER){
		cout << fixed << setprecision(5) << (end-start) << endl;
	}

	// deleting allocated memory
	delete ans;

	MPI_Finalize();
}