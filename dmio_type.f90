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
 
end module 
