# Conan build and deploy action

This action spins up a docker container and runs the installation tests inside

## Inputs

### `script`

**Required** The script to execute

## Example usage

```yaml

     steps:
      - uses: ./.github/actions/installation-test
        with:
          script: ./test/installation-tests.sh
        id: installation-tests
```