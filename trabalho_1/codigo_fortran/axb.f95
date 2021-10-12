program randomNumber
    implicit none
        real, dimension (:,:), allocatable :: A
        real, dimension (:), allocatable :: x , b 
        real :: time
        integer :: arrSize , i, j, k
        logical :: exists
        character(LEN =30) :: Format
        inquire(file="fortran_process_times_outside_i.txt", exist=exists)
    
        if(exists) then
            open(1,file = "fortran_process_times_outside_i.txt", status = "old", action = "write")
        else
            open(1,file = "fortran_process_times_outside_i.txt", status="new", action = "write")
        end if
    
        do k = 0, 30000, 1000
            allocate(A(k,k))
            allocate(x(k), b(k))
    
            call RANDOM_NUMBER(A)
            call RANDOM_NUMBER(x)
            do i = 1 , k
                do j = 1 , k 
                    b(i) = b(i) + A(j,i) * x(j)
                end do
            end do
            deallocate(A,b,x)
            call cpu_time(time)
            Format = "( I5,A,F10.6,A,A)"
            write(1,Format,ADVANCE="no" ) k,",",time,",","\n"
            Print*,k,time
        end do
        close(1)
    end program randomNumber
