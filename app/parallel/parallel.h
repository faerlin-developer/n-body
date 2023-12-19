#pragma once

#include <string>
#include <map>
#include <mpi.h>

namespace mpi {

    const int ROOT = 0;

    void init(int argc, char *argv[]);

    MPI_Comm split(const MPI_Comm &comm, int split_type, int key);

    int rank(const MPI_Comm &comm);

    int size(const MPI_Comm &comm);

    template<class T>
    MPI_Win allocate(const int size, const int disp_unit, const MPI_Comm &comm, T **baseptr) {
        MPI_Win window;
        MPI_Win_allocate_shared(size, disp_unit, MPI_INFO_NULL, comm, baseptr, &window);
        return window;
    }

    template<class T>
    void query(const MPI_Win &window, int rank, T **baseptr) {
        MPI_Aint size;
        int disp_unit;
        MPI_Win_shared_query(window, rank, &size, &disp_unit, baseptr);
    }

    void barrier(MPI_Comm comm);

    void free(MPI_Win window);

    void finalize();

};
