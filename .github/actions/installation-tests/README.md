# Conan build and deploy action

This action creates a conan package and deploys it to bintray

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