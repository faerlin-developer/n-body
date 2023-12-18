#pragma once

#include <string>
#include <map>
#include <mpi.h>

namespace mpi {

    const int ROOT = 0;

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

    void barrier(MPI_Comm comm) {
        MPI_Barrier(comm);
    }

    void free(MPI_Win window) {
        MPI_Win_free(&window);
    }

    void finalize() {
        MPI_Finalize();
    }

};
