program main
    use dmio_type
    implicit none
    integer:: proc_type
    client_num = 10
    call get_proc_type(5,proc_type)
    print*,proc_type
end program


