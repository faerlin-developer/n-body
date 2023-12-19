
#include <mpi.h>
#include "parallel/parallel.h"

namespace mpi {

    void init(int argc, char *argv[]) {
        MPI_Init(&argc, &argv);
    }

    MPI_Comm split(const MPI_Comm &comm, int split_type, int key) {
        MPI_Comm newcomm;
        MPI_Comm_split_type(comm, split_type, key, MPI_INFO_NULL, &newcomm);
        return newcomm;
    }

    int rank(const MPI_Comm &comm) {
        int rank;
        MPI_Comm_rank(comm, &rank);
        return rank;
    }

    int size(const MPI_Comm &comm) {
        int size;
        MPI_Comm_size(comm, &size);
        return size;
    }

    void barrier(MPI_Comm comm) {
        MPI_Barrier(comm);
    }

    void free(MPI_Win window) {
        MPI_Win_free(&window);
    }

    void finalize() {
        MPI_Finalize();
    }

}