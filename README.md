# pipex

**The "Pipex" project from 42 School simulates a shell pipeline, implementing custom piping, file redirection, and process management in C, demonstrating proficiency in system programming and Unix-based tools.**

- **Subject:** [English](subject/en.subject.pdf) / [French](subject/fr.subject.pdf) / [Spanish](subject/es.subject.pdf)
- **Location:** 42 School Barcelona
- **Validation:** January 2025

## How to use?

```bash
./pipex <input_file_path> <shell_command_1> <shell_command_2> <output_file_path>
```

Example:
```bash
./pipex test/infile "grep test" "wc -l" test/output
```

This command will give the same result as the following shell command:
```bash
< test/infile grep test | wc -c > test/outfile
```

For the given `infile` int the `test` folder, the both above commands will create `test/outfile` and write `3` inside.
