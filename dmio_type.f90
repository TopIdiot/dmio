module dmio_type
      implicit none
      save
      !define process type
      !proc_master is IO msg controler
      !proc_client is computing process
      !proc_server is IO process
      integer,parameter:: proc_master = 0
      integer,parameter:: proc_client = 1
      integer,parameter:: proc_server = 2
      !proc_num is the number is total processes 
      integer          :: proc_num 
      integer          :: client_num
      integer          :: server_num

      contains
          subroutine get_proc_type(rank,proc_type)
              implicit none
              integer,intent(in) :: rank 
              integer,intent(out):: proc_type

              if(rank == 0) then 
                   proc_type = proc_master
              else if (rank <= client_num) then
                  proc_type = proc_client
              else
                  proc_type =  proc_server
              end if
          end subroutine get_proc_type


end module dmio_type 
