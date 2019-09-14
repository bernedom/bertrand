# Conan build and deploy action

This action creates a conan package and deploys it to bintray

## Inputs

### `conan_package`

**Required** The name of the package (i.e. bertrand).

### `conan_channel` 

**Required** The channel to publish to (i.e. user/channel)

### `conan_login`

**Required** The username to use for the login to bintray. Preferably use a [secret](https://help.github.com/en/articles/virtual-environments-for-github-actions#creating-and-using-secrets-encrypted-variables) to pass this

### `conan_pass`

**Required** The password or API token for bintray. Again use a [secret](https://help.github.com/en/articles/virtual-environments-for-github-actions#creating-and-using-secrets-encrypted-variables) to pass this


### `conan_remote` 

**Required** The remote to push to


## Outputs

None

## Example usage

```yaml

 - uses: ./.github/actions/deploy-conan-to-bintray
        with:
          conan_package: bertrand
          conan_channel: bertrand/unstable
          conan_remote: "https://api.bintray.com/conan/bernedom/conan"
          conan_user: ${{secrets.CONAN_LOGIN_USERNAME}}
          conan_pass: ${{secrets.CONAN_PASSWORD}}
        id: conan-deploy
```