module queue
    type :: node
        integer :: data
        type(node), pointer :: next
    end type

    type :: linkqueue
        type(node), pointer :: front
        type(node), pointer :: rear 
    end type

    contains
        subroutine init_queue(queue)
            type(linkqueue) :: queue
            type(node),pointer :: init_node
            allocate(init_node)
            queue%front=>init_node
            queue%rear =>init_node
            queue%front%next = NULL() 
         end subroutine   
        
         integer function is_empty(queue)
             type(linkqueue) :: queue

             if(queue%front.eq. queue%rear) then
                 is_empty = 1
             else is_empty = 0
             end if
         end function


end module
