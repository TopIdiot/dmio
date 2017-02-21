program main
    use dmio_type
    include 'mpif.h'
    MPI_Init()

end program

subroutine dmio_init(proc_type )
      use dmio_type
      include 'mpif.h'
      implicit none
      integer :: ierr,rank,sz
      integer , allocatable,dimension(:) :: ranks
      integer :: intent(inout) proc_type
      call MPI_Comm_size(sz,ierr)
      call MPI_Comm_rank(MPI_Comm_world,rank,ierr)
      call get_client_type(rank,proc_type) 
      allocate(ranks(sz))
      do i=client_num,sz
        ranks[i] = i
      end do
      rank[sz+1] = 0

      call MPI_Comm_group(MPI_Comm_world , group , ierr)
      call MPI_Group_incl(group , sz-client_num+1 , only_server_group , ierr)
      call MPI_Comm_create(MPI_Comm_world , only_server_group , only_server_comm , ierr)
      call MPI_Group_incl(group , sz-client_num+2 , server_and_master_group , ierr)
      call MPI_Comm_create(MPI_Comm_world , server_and_master_group , server_and_master_comm) 

end subroutine

subroutine mat_save(filename, varname, A, nx, ny )

    use dmio_type
    include 'mpif.h'
    character(len=*),intent(in) :: filename
    character(len=*),intent(in) :: varname
    integer:: rank,proc_type,ierr,sz
    integer::loop_status = 0
    real,dimension(nx,ny),intent(in) :: A
    call MPI_Comm_size(MPI_Comm_world,sz,ierr)
    call MPI_Comm_rank(MPI_Comm_world,rank,ierr)
    call get_proc_type(rank,proc_type)
    
   
    if(proc_type == prco_master ) then
        if(loop_status == 0) then
            loop_staus = 1
            do while(true)
                MPI_Recv(MPI_Comm_world)
            end do 
            loop_status = 0

        end if



    else if(proc_type == proc_client) then



    else 



    end if

      
end subroutine mat_save
