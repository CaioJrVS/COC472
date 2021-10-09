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
    
        do k = 0, 25000, 1000
            allocate(A(k,k))
            Print*,sizeof(A)
            allocate(x(k), b(k))
    
            call RANDOM_NUMBER(A)
            call RANDOM_NUMBER(x)
            do i = 1 , arrSize
                do j = 1 , arrSize
                    b(i) = b(i) + A(j,i) * x(j)
                end do
            end do
            deallocate(A,b,x)
            call cpu_time(time)
            Format = "( I5,A,F10.6,A)"
            write(1,Format,ADVANCE="no" ) k,",",time,","
            Print*,k,time
        end do
        close(1)
    end program randomNumber