#!/usr/bin/env zsh

guess_language () {
    if [[ -f "${1}/Cargo.toml" ]]; then
        print Rust
    elif [[ -f "${1}/CMakeLists.txt" ]]; then
        if fgrep -q CXX "${1}/CMakeLists.txt"; then
            print C++
        elif fgrep -q "LANGUAGES C)" "${1}/CMakeLists.txt"; then
            print C
        else
            print Unknown CMake language in $1 >&2
        fi
    elif [[ -f "${1}/puzzle1.py" ]]; then
        print Python
    elif [[ -f "${1}/puzzle1.js" ]]; then
        print JavaScript
    else
        print Could not detect language in $1 >&2
    fi
}

build () {
    case $(guess_language $1) in
        Rust)   (
                cd $1
                cargo build --release
                )
                ;;
        C|C++)  (
                mkdir -p ${1}/build
                cd ${1}/build
                if [[ -f CMakeCache.txt ]] && grep -q 'CMAKE_BUILD_TYPE.*Debug' CMakeCache.txt; then
                    rm -rf *
                fi
                if ! [[ -f CMakeCache.txt ]]; then
                    cmake ..
                fi
                cmake --build . -j8
                )
                ;;
        *)      print "(Nothing to do)"
    esac
}

get_executables () {
    case $(guess_language $1) in
        Rust)       print "${1}"/target/release/puzzle{1,2}
                    ;;
        C|C++)      print "${1}"/build/puzzle{1,2}
                    ;;
        Python)     print "${1}"/puzzle{1,2}.py
                    ;;
        JavaScript) print "${1}"/puzzle{1,2}.js
    esac
}

run_and_time () {
    executable="${1}"
    input_file="${2}"
    # Do a test run to cache the executable and get the result
    result=$($executable <$input_file)
    # Do the time trial
    TIMEFMT="%*E"
    duration_1=$((time $executable <$input_file >/dev/null 2>&1) 2>&1)
    N=$(echo 0.5 / $duration_1 + 1 | bc)
    duration_n=$((time (
        for i in {1..$N}; do
            $executable <$input_file >/dev/null 2>&1
        done    
        )) 2>&1)
    (( duration_ms = $duration_n * 1000 / $N ))
    print $duration_ms $result
}

# First we need to find all solutions
aocroot="${0:a:h}"
cd "$aocroot"

dirs=($(print -l *(/) | egrep '[0-9]+' | sort -n))

for d in $dirs; do
    print "++++ " Building day $d
    build $d
done

print
print "+++++++++++++++++++++++++++++++++++++++++++++"
print

printf "%-4s %-8s %-12s %20s %10s\n" Day Puzzle Language Result "Time [ms]"
print

sum=0

for d in $dirs; do
    lang=$(guess_language $d)
    exes=($(get_executables "$d"))
    res1=($(run_and_time ${exes[1]} $d/input))
    res2=($(run_and_time ${exes[2]} $d/input))
    time1=${res1[1]}
    time2=${res2[1]}
    ans1=${res1[2]}
    ans2=${res2[2]}
    printf "%-4s %-8s %-12s %20s %10.1f\n" $d 1 $lang $ans1 $time1
    printf "%-4s %-8s %-12s %20s %10.1f\n" $d 2 $lang $ans2 $time2
    (( sum = sum + $time1 + $time2 ))
done

printf "%-4s %-8s %-12s %-20s %10.1f\n" "" "" "" "SUM" $sum
